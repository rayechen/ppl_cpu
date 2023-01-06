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

#ifndef __ST_PPL_KERNEL_X86_FP32_CONV_H_
#define __ST_PPL_KERNEL_X86_FP32_CONV_H_

#include "ppl/kernel/x86/common/general_include.h"
#include "ppl/kernel/x86/common/conv_common.h"

namespace ppl { namespace kernel { namespace x86 {

uint64_t conv2d_ndarray_fp32_get_buffer_bytes(
    const ppl::common::isa_t isa,
    const ppl::common::TensorShape *dst_shape,
    const int64_t group,
    const int64_t channels,
    const int64_t kernel_h,
    const int64_t kernel_w,
    const int64_t stride_h,
    const int64_t stride_w,
    const int64_t pad_h,
    const int64_t pad_w);

ppl::common::RetCode conv2d_ndarray_fp32(
    const ppl::common::isa_t isa,
    const ppl::common::TensorShape *src_shape,
    const ppl::common::TensorShape *sum_src_shape,
    const ppl::common::TensorShape *dst_shape,
    const float *src,
    const float *sum_src,
    const float *filter,
    const float *bias,
    const int64_t group,
    const int64_t channels,
    const int64_t num_output,
    const int64_t kernel_h,
    const int64_t kernel_w,
    const int64_t stride_h,
    const int64_t stride_w,
    const int64_t pad_h,
    const int64_t pad_w,
    const int64_t hole_h,
    const int64_t hole_w,
    const conv_fuse_flag_t fuse_flag,
    void *temp_buffer,
    float *dst);

uint64_t conv1d_ndarray_fp32_get_buffer_bytes(
    const ppl::common::isa_t isa,
    const ppl::common::TensorShape *dst_shape,
    const int64_t group,
    const int64_t channels,
    const int64_t kernel_w,
    const int64_t stride_w,
    const int64_t pad_w);

ppl::common::RetCode conv1d_ndarray_fp32(
    const ppl::common::isa_t isa,
    const ppl::common::TensorShape *src_shape,
    const ppl::common::TensorShape *sum_src_shape,
    const ppl::common::TensorShape *dst_shape,
    const float *src,
    const float *sum_src,
    const float *filter,
    const float *bias,
    const int64_t group,
    const int64_t channels,
    const int64_t num_output,
    const int64_t kernel_w,
    const int64_t stride_w,
    const int64_t pad_w,
    const int64_t hole_w,
    const conv_fuse_flag_t fuse_flag,
    void *temp_buffer,
    float *dst);

}}}; // namespace ppl::kernel::x86

#endif
