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

#ifndef __ST_PPL_KERNEL_RISCV_FP32_CONV2D_COMMON_GEMM_KERNEL_CONV2D_GEMM_KERNEL_NDARRAY_N4CX_FP32_H_
#define __ST_PPL_KERNEL_RISCV_FP32_CONV2D_COMMON_GEMM_KERNEL_CONV2D_GEMM_KERNEL_NDARRAY_N4CX_FP32_H_

namespace ppl { namespace kernel { namespace riscv {

template <int64_t m, int64_t n, bool first>
static void gemm_kernel_m16n7_ndarray_n4cx_fp32_vec128(
    const float* a,
    const float* b,
    float* c,
    const int64_t total_m,
    const int64_t total_n,
    const int64_t total_k)
{
    asm volatile(
        ".equ            IS_FIRST, %c[IS_FIRST]     \n\t"
        ".equ            ATOM_M, %c[ATOM_M]         \n\t"
        ".equ            ATOM_N, %c[ATOM_N]         \n\t"

        "addi            s3, zero, 4                \n\t"
        "vsetvli         s2, s3, e32                \n\t"

        "mv              s2, %[A_LOC]               \n\t"
        "mv              s3, %[B_LOC]               \n\t"
        "mv              s4, %[C_LOC]               \n\t"
        "mv              s5, %[K]                   \n\t"

        "0:                                         \n\t" // init
        ".if IS_FIRST == 1                          \n\t"
        "addi            s5, s5, -1                 \n\t"
        ".if ATOM_M > 0                             \n\t"
        "vle.v           v0, (s2)                   \n\t"
        "add             s2, s2, %[A_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 4                             \n\t"
        "vle.v           v1, (s2)                   \n\t"
        "add             s2, s2, %[A_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 8                             \n\t"
        "vle.v           v2, (s2)                   \n\t"
        "add             s2, s2, %[A_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 12                            \n\t"
        "vle.v           v3, (s2)                   \n\t"
        "add             s2, s2, %[A_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        "sub             s2, s2, %[A_BACK_STRIDE]   \n\t"
        ".if ATOM_N > 0                             \n\t"
        "flw             f0, 0(s3)                  \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "flw             f1, 4(s3)                  \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "flw             f2, 8(s3)                  \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "flw             f3, 12(s3)                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "flw             f4, 16(s3)                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "flw             f5, 20(s3)                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "flw             f6, 24(s3)                 \n\t"
        ".endif                                     \n\t"

        ".if ATOM_M > 0                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vfmul.vf        v4, v0, f0                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vfmul.vf        v5, v0, f1                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vfmul.vf        v6, v0, f2                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vfmul.vf        v7, v0, f3                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vfmul.vf        v8, v0, f4                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vfmul.vf        v9, v0, f5                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vfmul.vf        v10, v0, f6                \n\t"
        ".endif                                     \n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 4                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vfmul.vf        v11, v1, f0                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vfmul.vf        v12, v1, f1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vfmul.vf        v13, v1, f2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vfmul.vf        v14, v1, f3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vfmul.vf        v15, v1, f4                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vfmul.vf        v16, v1, f5                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vfmul.vf        v17, v1, f6                \n\t"
        ".endif                                     \n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 8                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vfmul.vf        v18, v2, f0                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vfmul.vf        v19, v2, f1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vfmul.vf        v20, v2, f2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vfmul.vf        v21, v2, f3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vfmul.vf        v22, v2, f4                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vfmul.vf        v23, v2, f5                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vfmul.vf        v24, v2, f6                \n\t"
        ".endif                                     \n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 12                            \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vfmul.vf        v25, v3, f0                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vfmul.vf        v26, v3, f1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vfmul.vf        v27, v3, f2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vfmul.vf        v28, v3, f3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vfmul.vf        v29, v3, f4                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vfmul.vf        v30, v3, f5                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vfmul.vf        v31, v3, f6                \n\t"
        ".endif                                     \n\t"
        ".endif                                     \n\t"

        "add             s3, s3, %[B_NXT_LINE_STRIDE]\n\t"
        "beq             s5, zero, 2f               \n\t"
        ".else                                      \n\t"
        ".if ATOM_M > 0                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vle.v           v4, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vle.v           v5, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vle.v           v6, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vle.v           v7, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vle.v           v8, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vle.v           v9, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vle.v           v10, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        "add             s4, s4, %[C_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 4                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vle.v           v11, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vle.v           v12, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vle.v           v13, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vle.v           v14, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vle.v           v15, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vle.v           v16, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vle.v           v17, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        "add             s4, s4, %[C_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 8                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vle.v           v18, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vle.v           v19, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vle.v           v20, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vle.v           v21, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vle.v           v22, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vle.v           v23, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vle.v           v24, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        "add             s4, s4, %[C_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 12                            \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vle.v           v25, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vle.v           v26, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vle.v           v27, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vle.v           v28, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vle.v           v29, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vle.v           v30, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vle.v           v31, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        "add             s4, s4, %[C_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        "mv              s4, %[C_LOC]               \n\t"
        ".endif                                     \n\t"

        "1:                                             \n\t" // loop k
        "addi            s5, s5, -1                 \n\t"
        ".if ATOM_M > 0                             \n\t"
        "vle.v           v0, (s2)                   \n\t"
        "add             s2, s2, %[A_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 4                             \n\t"
        "vle.v           v1, (s2)                   \n\t"
        "add             s2, s2, %[A_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 8                             \n\t"
        "vle.v           v2, (s2)                   \n\t"
        "add             s2, s2, %[A_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 12                            \n\t"
        "vle.v           v3, (s2)                   \n\t"
        "add             s2, s2, %[A_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        "sub             s2, s2, %[A_BACK_STRIDE]   \n\t"
        ".if ATOM_N > 0                             \n\t"
        "flw             f0, 0(s3)                  \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "flw             f1, 4(s3)                  \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "flw             f2, 8(s3)                  \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "flw             f3, 12(s3)                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "flw             f4, 16(s3)                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "flw             f5, 20(s3)                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "flw             f6, 24(s3)                 \n\t"
        ".endif                                     \n\t"

        ".if ATOM_M > 0                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vfmacc.vf       v4, f0, v0                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vfmacc.vf       v5, f1, v0                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vfmacc.vf       v6, f2, v0                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vfmacc.vf       v7, f3, v0                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vfmacc.vf       v8, f4, v0                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vfmacc.vf       v9, f5, v0                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vfmacc.vf       v10, f6, v0                \n\t"
        ".endif                                     \n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 4                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vfmacc.vf       v11, f0, v1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vfmacc.vf       v12, f1, v1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vfmacc.vf       v13, f2, v1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vfmacc.vf       v14, f3, v1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vfmacc.vf       v15, f4, v1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vfmacc.vf       v16, f5, v1                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vfmacc.vf       v17, f6, v1                \n\t"
        ".endif                                     \n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 8                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vfmacc.vf       v18, f0, v2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vfmacc.vf       v19, f1, v2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vfmacc.vf       v20, f2, v2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vfmacc.vf       v21, f3, v2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vfmacc.vf       v22, f4, v2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vfmacc.vf       v23, f5, v2                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vfmacc.vf       v24, f6, v2                \n\t"
        ".endif                                     \n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 12                            \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vfmacc.vf       v25, f0, v3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vfmacc.vf       v26, f1, v3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vfmacc.vf       v27, f2, v3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vfmacc.vf       v28, f3, v3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vfmacc.vf       v29, f4, v3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vfmacc.vf       v30, f5, v3                \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vfmacc.vf       v31, f6, v3                \n\t"
        ".endif                                     \n\t"
        ".endif                                     \n\t"

        "add             s3, s3, %[B_NXT_LINE_STRIDE]\n\t"
        "bnez            s5, 1b                     \n\t"

        "2:                                             \n\t" // end
        ".if ATOM_M > 0                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vse.v           v4, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vse.v           v5, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vse.v           v6, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vse.v           v7, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vse.v           v8, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vse.v           v9, (s4)                   \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vse.v           v10, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        "add             s4, s4, %[C_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 4                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vse.v           v11, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vse.v           v12, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vse.v           v13, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vse.v           v14, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vse.v           v15, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vse.v           v16, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vse.v           v17, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        "add             s4, s4, %[C_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 8                             \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vse.v           v18, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vse.v           v19, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vse.v           v20, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vse.v           v21, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vse.v           v22, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vse.v           v23, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vse.v           v24, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        "add             s4, s4, %[C_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"
        ".if ATOM_M > 12                            \n\t"
        ".if ATOM_N > 0                             \n\t"
        "vse.v           v25, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 1                             \n\t"
        "vse.v           v26, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 2                             \n\t"
        "vse.v           v27, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 3                             \n\t"
        "vse.v           v28, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 4                             \n\t"
        "vse.v           v29, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 5                             \n\t"
        "vse.v           v30, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        ".if ATOM_N > 6                             \n\t"
        "vse.v           v31, (s4)                  \n\t"
        "add             s4, s4, 16                 \n\t"
        ".endif                                     \n\t"
        "add             s4, s4, %[C_NXT_LINE_STRIDE]\n\t"
        ".endif                                     \n\t"

        :
        : [ATOM_M] "i"(m), [ATOM_N] "i"(n), [IS_FIRST] "i"(first), [A_LOC] "r"(a), [B_LOC] "r"(b), [C_LOC] "r"(c), [K] "r"(total_k), [A_NXT_LINE_STRIDE] "r"(total_k * 16), [A_BACK_STRIDE] "r"(m / 4 * total_k * 16 - 16), [B_NXT_LINE_STRIDE] "r"(total_n * 4), [C_NXT_LINE_STRIDE] "r"((total_n - n) * 16)

        : "memory", "s2", "s3", "s4", "s5");
}

template <int64_t align_m, int64_t align_n, int64_t left_m, int64_t left_n, bool first>
void gemm_ndarray_n4cx_fp32_vec128(
    const float* a,
    const float* b,
    float* c,
    const int64_t total_m,
    const int64_t total_n,
    const int64_t total_k)
{
    const int64_t atom_k           = 1;
    const int64_t atom_m           = 4;
    const int64_t c_nxt_blk_stride = (align_m - atom_m) * total_n;
    const int64_t a_nxt_blk_stride = align_m * total_k;

    int64_t mi          = 0;
    const float* temp_a = a;
    float* temp_c       = c;
    for (mi = 0; mi <= total_m - align_m; mi += align_m) {
        int64_t ni          = 0;
        const float* temp_b = b;
        for (ni = 0; ni <= total_n - align_n; ni += align_n) {
            gemm_kernel_m16n7_ndarray_n4cx_fp32_vec128<align_m, align_n, first>(temp_a, temp_b, temp_c, total_m, total_n, total_k);
            temp_b += atom_k * align_n;
            temp_c += atom_m * align_n;
        }
        if (ni < total_n) {
            gemm_kernel_m16n7_ndarray_n4cx_fp32_vec128<align_m, left_n, first>(temp_a, temp_b, temp_c, total_m, total_n, total_k);
            temp_c += atom_m * left_n;
        }
        temp_c += c_nxt_blk_stride;
        temp_a += a_nxt_blk_stride;
    }
    if (mi < total_m) {
        int64_t ni          = 0;
        const float* temp_b = b;
        for (ni = 0; ni <= total_n - align_n; ni += align_n) {
            gemm_kernel_m16n7_ndarray_n4cx_fp32_vec128<left_m, align_n, first>(temp_a, temp_b, temp_c, total_m, total_n, total_k);
            temp_b += atom_k * align_n;
            temp_c += atom_m * align_n;
        }
        if (ni < total_n) {
            gemm_kernel_m16n7_ndarray_n4cx_fp32_vec128<left_m, left_n, first>(temp_a, temp_b, temp_c, total_m, total_n, total_k);
        }
    }
}

}}}; // namespace ppl::kernel::riscv

#endif