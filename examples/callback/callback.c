// Copyright (c) 2017 Sony Corporation. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <nnablart/network.h>
#include <nnablart/runtime.h>

#include "context.h"

#include <assert.h>
#include <string.h>

/// @defgroup Examples Examples
/// @{

/// @file
/// @brief Simple callback example.

/// @brief NNB format binary data from Affine_000.nntxt
///
/// To use callback, you must set 'function implement flag' in NNB file.
/// You can get template setting file with following command.
/// @code{.sh}
/// $ cp build/test/nnabla/succeed/Affine_000.nntxt .
/// $ nnabla_cli nnb_template Affine_000.nntxt Affine_000.yaml
/// @endcode
///
/// Then you should be got followint 'Affine_000.yaml'.
/// @code{.yaml}
/// functions:
///   Affine:
///     implement: 0
/// variables:
///   x: FLOAT32
///   weight: FLOAT32
///   bias: FLOAT32
///   y0: FLOAT32
/// @endcode
///
/// Edit 'Affine_000.yaml' and save as 'settings.yaml'
/// @code{.yaml}
/// functions:
///   Affine:
///     implement: 100
/// @endcode
///
/// And convert nntxt to nnb again with settings.yaml.
/// @code{.sh}
/// $ nnabla_cli convert -s settings.yaml
/// nnabla_cli convert -s settings.yaml Affine_000.nntxt Affine_000.nnb
/// @endcode
///
/// Then we create binary data with following command.
/// @code{.sh}
/// $ xxd -i <Affine_000.nnb >Affine_000.c
/// @endcode
///
static unsigned char affine_nnb[] = {
    0x3f, 0x20, 0x21, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x11, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00,
    0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x34, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00,
    0x74, 0x01, 0x00, 0x00, 0x7c, 0x01, 0x00, 0x00, 0x94, 0x01, 0x00, 0x00,
    0xa8, 0x01, 0x00, 0x00, 0xb8, 0x01, 0x00, 0x00, 0xcc, 0x01, 0x00, 0x00,
    0xdc, 0x01, 0x00, 0x00, 0xe8, 0x01, 0x00, 0x00, 0xec, 0x01, 0x00, 0x00,
    0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x0c, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x64, 0x7c, 0x1d, 0xc0,
    0xb9, 0xaf, 0x29, 0x3f, 0xc9, 0x4c, 0x72, 0xbf, 0xf3, 0x54, 0x0b, 0x40,
    0x96, 0x6d, 0x57, 0x3f, 0x2a, 0x74, 0xe0, 0x3e, 0xfb, 0x20, 0x23, 0xbe,
    0x1d, 0x31, 0x6c, 0x3e, 0xd8, 0x50, 0x4f, 0xbf, 0x5f, 0x99, 0x01, 0xc0,
    0x32, 0x3b, 0x16, 0xc0, 0x14, 0x2b, 0xb5, 0xbd, 0x02, 0x8e, 0xb1, 0x3e,
    0x2f, 0xda, 0x0e, 0xbe, 0x9b, 0x67, 0x5f, 0xbf, 0x89, 0xe6, 0x37, 0xbf,
    0xc5, 0xe9, 0x0a, 0xbf, 0x7c, 0x69, 0x4e, 0x3f, 0x92, 0x40, 0xf6, 0xbf,
    0x8e, 0x8e, 0x83, 0x3e, 0xb6, 0xdb, 0x42, 0x3e, 0x14, 0x8d, 0xb8, 0x3f,
    0xdd, 0x5c, 0x08, 0x3f, 0x2b, 0x36, 0x19, 0xbf, 0xa6, 0xc3, 0x89, 0x3e,
    0xe0, 0x8f, 0xbd, 0xbe, 0xf4, 0x6b, 0xb8, 0x3e, 0x67, 0x95, 0xcd, 0x3f,
    0xb2, 0xce, 0x04, 0xbf, 0x79, 0x9e, 0x88, 0xbd, 0x00, 0x0e, 0xfb, 0x3d,
    0xba, 0x81, 0xb4, 0x3f, 0x99, 0x5c, 0x58, 0xbe, 0xe5, 0x6d, 0x52, 0xbf,
    0x9b, 0x62, 0xa1, 0xbe, 0x8f, 0xad, 0x7e, 0xbd, 0x3b, 0x81, 0x91, 0x3f,
    0x3b, 0x30, 0xb9, 0x3e, 0xe5, 0x69, 0xaa, 0xbf, 0x5f, 0xf1, 0xd6, 0x3f,
    0x3b, 0xa6, 0xc1, 0x3f, 0x6d, 0x5f, 0xe0, 0xbe, 0xd9, 0xec, 0xa2, 0x3d,
    0x0e, 0xe6, 0x83, 0x3f, 0x37, 0x06, 0xa3, 0x3f, 0xd8, 0x3f, 0x5c, 0x3f,
    0xb7, 0x33, 0x34, 0x3f, 0x96, 0xf4, 0x13, 0xbf, 0xca, 0xfd, 0x41, 0x3f,
    0x1e, 0xcc, 0xf3, 0x3e, 0x6e, 0x06, 0x1a, 0xc0, 0xb6, 0x20, 0x60, 0xbe,
    0x70, 0x39, 0x21, 0xbf, 0x25, 0xcf, 0x82, 0xbf, 0xc9, 0xb6, 0x22, 0x3f,
    0x2a, 0x65, 0x33, 0x3e, 0xcc, 0xd0, 0xb7, 0x3f, 0x95, 0xcd, 0xa4, 0x3c,
    0xe8, 0x00, 0x3f, 0x3e, 0xe3, 0xc4, 0x4b, 0x3f, 0x2c, 0x34, 0x8b, 0x3e,
    0x2d, 0x14, 0xce, 0x3f, 0x0a, 0x1f, 0xac, 0x3f, 0x29, 0xee, 0x58, 0x3f,
    0x45, 0x24, 0xd0, 0x3e, 0x74, 0x97, 0x9e, 0xbe, 0xdd, 0xe3, 0xfd, 0xbd,
    0x06, 0xa9, 0xe2, 0xbf, 0x7d, 0xd3, 0x0e, 0x3f, 0x13, 0x12, 0x67, 0x3f,
    0xec, 0x2d, 0x7a, 0xbf, 0x29, 0x1d, 0xee, 0x3e, 0x01, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0xb2, 0xa6, 0xdb, 0x3f, 0x90, 0x54, 0x8a, 0xbf, 0x5a, 0xbd, 0x45, 0x3e,
    0x26, 0x88, 0x2d, 0xbf, 0x02, 0x6a, 0xca, 0x3e, 0x14, 0x70, 0x79, 0xbf,
    0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x0e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};

/// @brief dummy input data.
static unsigned char input[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};

/// @brief Example callback for execute function.
/// It does nothing but just print function name itself.
static rt_function_error_t cb_exec(rt_function_t *f) {
  WHOAMI("%s", __func__);
  return RT_FUNCTION_ERROR_NOERROR;
}

/// @brief Example callback for free function local context.
/// It does nothing but just print function name itself.
static rt_function_error_t cb_free(rt_function_t *f) {
  WHOAMI("%s", __func__);
  return RT_FUNCTION_ERROR_NOERROR;
}

/// @brief Example callback for allocate function local context.
/// It just does followings.
/// - check func->imple == 100
/// - register @ref cb_exec as function executtor.
/// - register @ref cb_free as function local context de-allocator.
///
/// See also @ref rt_initialize_context
static rt_return_value_t cb_alloc(nn_network_t *net, void *function_context) {
  WHOAMI("%s", __func__);
  rt_function_context_t *func = (rt_function_context_t *)function_context;
  if ((int)func->info->impl != 100) {
    return RT_RET_FUNCTION_DONT_MATCH;
  }

  func->func.exec_func = cb_exec;
  func->func.free_local_context_func = cb_free;
  func->func.local_context = 0;
  return RT_RET_FUNCTION_MATCH;
}

/// @brief Simple example to use user defined functions.
int main(int argc, char *argv[]) {
  WHOAMI("Callback test.");
  rt_context_pointer context = 0;

  rt_return_value_t ret = rt_allocate_context(&context);
  assert(ret == RT_RET_NOERROR);

  ret = rt_add_callback(context, NN_FUNCTION_AFFINE, cb_alloc);
  assert(ret == RT_RET_NOERROR);

  nn_network_t *net = (nn_network_t *)affine_nnb;
  ret = rt_initialize_context(context, net);
  assert(ret == RT_RET_NOERROR);

  assert(rt_num_of_input(context) == 1);
  assert(rt_input_size(context, 0) == sizeof(input));
  memcpy(rt_input_buffer(context, 0), input, sizeof(input));

  ret = rt_forward(context);
  assert(ret == RT_RET_NOERROR);

  ret = rt_free_context(&context);
  assert(ret == RT_RET_NOERROR);

  return 0;
}

/// @}
