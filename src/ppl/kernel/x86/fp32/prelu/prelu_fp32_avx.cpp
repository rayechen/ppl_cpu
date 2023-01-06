// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include <immintrin.h>

#include "ppl/kernel/x86/common/internal_include.h"
#include "ppl/kernel/x86/common/avx_tools.h"

namespace ppl { namespace kernel { namespace x86 {

ppl::common::RetCode prelu_per_channel_ndarray_fp32_avx(
    const ppl::common::TensorShape *src_shape,
    const float *src,
    const float *slope,
    float *dst)
{
    const int64_t batch    = src_shape->GetDim(0);
    const int64_t channels = src_shape->GetDimCount() > 1 ? src_shape->GetDim(1) : 1;
    int64_t inner_dim = 1;
    for (uint32_t i = 2; i < src_shape->GetDimCount(); ++i) {
        inner_dim *= src_shape->GetDim(i);
    }

    const int64_t V_REG_ELTS   = 8;
    const int64_t unroll_inner = 2 * V_REG_ELTS;

#ifndef PPL_USE_X86_OMP_COLLAPSE
    PRAGMA_OMP_PARALLEL_FOR()
#else
    PRAGMA_OMP_PARALLEL_FOR_COLLAPSE(2)
#endif
    for (int64_t n = 0; n < batch; ++n) {
        for (int64_t c = 0; c < channels; ++c) {
            auto slope_var = slope[c];
            auto base_dst = dst + n * channels * inner_dim + c * inner_dim;
            auto base_src = src + n * channels * inner_dim + c * inner_dim;
            int64_t i = 0;

            if (inner_dim >= unroll_inner) {
                const auto v_zero     = _mm256_setzero_ps();
                const auto slope_vec0 = _mm256_set1_ps(slope_var);
                for (; i <= inner_dim - unroll_inner; i += unroll_inner) {
                    auto v_src0 = _mm256_loadu_ps(base_src + i + V_REG_ELTS * 0);
                    auto v_src1 = _mm256_loadu_ps(base_src + i + V_REG_ELTS * 1);

                    auto v_ge0 = _mm256_max_ps(v_src0, v_zero);
                    auto v_ge1 = _mm256_max_ps(v_src1, v_zero); 

                    auto v_le0 = _mm256_mul_ps(_mm256_min_ps(v_src0, v_zero), slope_vec0);
                    auto v_le1 = _mm256_mul_ps(_mm256_min_ps(v_src1, v_zero), slope_vec0);

                    _mm256_storeu_ps(base_dst + i + V_REG_ELTS * 0, _mm256_add_ps(v_ge0, v_le0));
                    _mm256_storeu_ps(base_dst + i + V_REG_ELTS * 1, _mm256_add_ps(v_ge1, v_le1));
                }
            }
            for (; i < inner_dim; ++i) {
                base_dst[i] = base_src[i] >= 0 ? base_src[i] : slope_var * base_src[i];
            }
        }
    }

    return ppl::common::RC_SUCCESS;
}

ppl::common::RetCode prelu_per_channel_n16cx_fp32_avx(
    const ppl::common::TensorShape *src_shape,
    const float *src,
    const float *slope,
    float *dst)
{
    const int64_t V_REG_ELTS   = 8;
    const int64_t CH_DATA_BLK  = 16;

    const int64_t batch    = src_shape->GetDim(0);
    const int64_t channels = src_shape->GetDimCount() > 1 ? src_shape->GetDim(1) : 1;
    const int64_t padded_c = round_up(channels, CH_DATA_BLK);
    int64_t inner_dim = 1;
    for (uint32_t i = 2; i < src_shape->GetDimCount(); ++i) {
        inner_dim *= src_shape->GetDim(i);
    }

#ifndef PPL_USE_X86_OMP_COLLAPSE
    PRAGMA_OMP_PARALLEL_FOR()
#else
    PRAGMA_OMP_PARALLEL_FOR_COLLAPSE(2)
#endif
    for (int64_t n = 0; n < batch; ++n) {
        for (int64_t c = 0; c < channels; c += CH_DATA_BLK) {
            const int64_t c_eff = min<int64_t>(channels - c, CH_DATA_BLK);
            auto base_slope = slope + c;
            auto base_dst = dst + n * padded_c * inner_dim + c * inner_dim;
            auto base_src = src + n * padded_c * inner_dim + c * inner_dim;

            float slope_buf[CH_DATA_BLK] = {0};
            if (c_eff != CH_DATA_BLK) {
                memcpy32_avx(slope_buf, base_slope, c_eff);
                base_slope = slope_buf;
            }

            const auto v_zero     = _mm256_setzero_ps();
            const auto slope_vec0 = _mm256_loadu_ps(base_slope + 0 * V_REG_ELTS);
            const auto slope_vec1 = _mm256_loadu_ps(base_slope + 1 * V_REG_ELTS);
            int64_t i = inner_dim * CH_DATA_BLK;
            while (i > 0) {
                i -= CH_DATA_BLK;
                auto v_src0 = _mm256_loadu_ps(base_src + i + V_REG_ELTS * 0);
                auto v_src1 = _mm256_loadu_ps(base_src + i + V_REG_ELTS * 1);

                auto v_ge0 = _mm256_max_ps(v_src0, v_zero);
                auto v_ge1 = _mm256_max_ps(v_src1, v_zero);

                auto v_le0 = _mm256_mul_ps(_mm256_min_ps(v_src0, v_zero), slope_vec0);
                auto v_le1 = _mm256_mul_ps(_mm256_min_ps(v_src1, v_zero), slope_vec1);

                _mm256_storeu_ps(base_dst + i + V_REG_ELTS * 0, _mm256_add_ps(v_ge0, v_le0));
                _mm256_storeu_ps(base_dst + i + V_REG_ELTS * 1, _mm256_add_ps(v_ge1, v_le1));
            }
        }
    }

    return ppl::common::RC_SUCCESS;
}

}}}; // namespace ppl::kernel::x86