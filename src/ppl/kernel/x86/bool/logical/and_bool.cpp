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

#include "logical_bool_common.h"

namespace ppl { namespace kernel { namespace x86 {

ppl::common::RetCode and_eltwise_bool(
    const ppl::common::TensorShape *dst_shape,
    const uint8_t *src0,
    const uint8_t *src1,
    uint8_t *dst)
{
    return logical_eltwise_binary_op_bool<LOGICAL_AND>(dst_shape, src0, src1, dst);
}

ppl::common::RetCode and_ndarray_bool(
    const ppl::common::TensorShape *src0_shape,
    const ppl::common::TensorShape *src1_shape,
    const ppl::common::TensorShape *dst_shape,
    const uint8_t *src0,
    const uint8_t *src1,
    uint8_t *dst)
{
    return logical_ndarray_binary_op_bool<LOGICAL_AND>(src0_shape, src1_shape, dst_shape, src0, src1, dst);
}

}}}; // namespace ppl::kernel::x86