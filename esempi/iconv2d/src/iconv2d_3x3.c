// Copyright 2020 ETH Zurich and University of Bologna.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Author: Matteo Perotti

#include "../inc/iconv2d.h"
#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))



void iconv2d_3x3(int32_t *o, int32_t *i, int32_t *f, int64_t R, int64_t C,
                 int64_t F) {

  // We work on 4 rows of the output matrix at once
  int32_t block_size_o = 4;
  // We work on block_size_o + F - 1 rows of the input matrix at once

  // First iteration round, r = 0
  int32_t *i_ = i;
  int32_t *o_ = o;

  
  // Temporary variables
  int t0, t1, t2, t3, t4, t5, t6, t7, t8;
  int32_t ldi = (C + F - 1) << 2;
  int32_t ldf = F << 2;
  int32_t *f_;
  f_ = f;
  asm volatile("ld %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t0) : "r"(ldf));
  asm volatile("ld %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t1) : "r"(ldf));
  asm volatile("ld %1, (%0);" : "+&r"(f_), "=&r"(t2));
  f_ = f + 1;
  // Fetch the middle column of the filter, and start calculating its
  // contributions on the output rows To do so, slide down the input rows by one
  asm volatile("ld %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t3) : "r"(ldf));
  asm volatile("ld %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t4) : "r"(ldf));
  asm volatile("ld %1, (%0);" : "+&r"(f_), "=&r"(t5));
  f_ = f + 2;
  // Repeat for the last filter column, and then store the output rows
  asm volatile("ld %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t6) : "r"(ldf));
  asm volatile("ld %1, (%0); add %0, %0, %2" : "+&r"(f_), "=&r"(t7) : "r"(ldf));
  asm volatile("ld %1, (%0);" : "+&r"(f_), "=&r"(t8));
 
for (int32_t r = 0; r < R; r += block_size_o) {

  

    // Fetch C + F - 1 elements (padding included)
  int32_t ldo = C << 2;
  int32_t *i___=i+r*(C+F-1);
  int32_t *i_col = i + r*(C+F-1) + 2;
  
  // Fetch C + F - 1 elements (padding included)
  asm volatile("vsetvli zero, %0, e32, m2, ta, ma" ::"r"(C + F - 1));
  asm volatile("vmv.v.i v0, 0");
  asm volatile("vmv.v.i v2, 0");
  asm volatile("vmv.v.i v4, 0");
  asm volatile("vmv.v.i v6, 0");
  
  asm volatile("vle32.v v8,  (%0); add %0, %0, %1" : "+&r"(i___) : "r"(ldi));
  
  asm volatile("vle32.v v10, (%0); add %0, %0, %1" : "+&r"(i___) : "r"(ldi));
  

  asm volatile("vle32.v v12, (%0); add %0, %0, %1" : "+&r"(i___) : "r"(ldi));
  

  asm volatile("vmul.vx v0, v8, %0" ::"r"(t0));
  
  asm volatile("vmul.vx v2, v10, %0" ::"r"(t0));
  asm volatile("vle32.v v14, (%0); add %0, %0, %1" : "+&r"(i___) : "r"(ldi));
  
  asm volatile("vmacc.vx v0, %0, v10" ::"r"(t1));

  asm volatile("vmacc.vx v2, %0, v12" ::"r"(t1));
  asm volatile("vle32.v v16, (%0); add %0, %0, %1" : "+&r"(i___) : "r"(ldi));
  
  asm volatile("vmacc.vx v0, %0, v12" ::"r"(t2));
  
  asm volatile("vslidedown.vi v20, v8,  1");
  asm volatile("vmul.vx v4, v12, %0" ::"r"(t0));

  asm volatile("vle32.v v18, (%0); add %0, %0, %1" : "+&r"(i___) : "r"(ldi));

  asm volatile("vsetvli zero, %0, e32, m2, ta, ma" ::"r"(C));

  asm volatile("vmul.vx v6, v14, %0" ::"r"(t0));
  asm volatile("vmacc.vx v4, %0, v14" ::"r"(t1));
  asm volatile("vslidedown.vi v22, v10, 1");
  asm volatile("vmacc.vx v2, %0, v14" ::"r"(t2));

  asm volatile("vmacc.vx v6, %0, v16" ::"r"(t1));
  asm volatile("vmacc.vx v4, %0, v16" ::"r"(t2));

  asm volatile("vslidedown.vi v24, v12, 1");
  asm volatile("vmacc.vx v6, %0, v18" ::"r"(t2));


  asm volatile("vmacc.vx v0, %0, v20" ::"r"(t3));

  asm volatile("vmacc.vx v0, %0, v22" ::"r"(t4));
  asm volatile("vslidedown.vi v26, v14, 1");
  asm volatile("vmacc.vx v2, %0, v22" ::"r"(t3));

  asm volatile("vmacc.vx v0, %0, v24" ::"r"(t5));
  asm volatile("vmacc.vx v2, %0, v24" ::"r"(t4));
  asm volatile("vslidedown.vi v28, v16, 1");
  asm volatile("vmacc.vx v4, %0, v24" ::"r"(t3));

  asm volatile("vmacc.vx v2, %0, v26" ::"r"(t5));
  asm volatile("vmacc.vx v4, %0, v26" ::"r"(t4));
  asm volatile("vslidedown.vi v30, v18, 1");
  asm volatile("vmacc.vx v6, %0, v26" ::"r"(t3));

  asm volatile("vmacc.vx v4, %0, v28" ::"r"(t5));
  asm volatile("vslidedown.vi v20, v8,  2");
  
  
  
  asm volatile("vmacc.vx v6, %0, v28" ::"r"(t4));

  asm volatile("vmacc.vx v6, %0, v30" ::"r"(t5));

  asm volatile("vslidedown.vi v22, v10,  2");


  asm volatile("vmacc.vx v0, %0, v20" ::"r"(t6));

  asm volatile("vmacc.vx v0, %0, v22" ::"r"(t7));
 
  asm volatile("vslidedown.vi v24, v12,  2");
  asm volatile("vmacc.vx v2, %0, v22" ::"r"(t6));

  // Compute on C elements

  asm volatile("vmacc.vx v0, %0, v24" ::"r"(t8));
  
  asm volatile("vse32.v  v0, (%0); add %0, %0, %1" : "+&r"(o_) : "r"(ldo));
  
  asm volatile("vslidedown.vi v26, v14,  2");
  asm volatile("vmacc.vx v2, %0, v24" ::"r"(t7));
  asm volatile("vmacc.vx v4, %0, v24" ::"r"(t6));

  asm volatile("vmacc.vx v2, %0, v26" ::"r"(t8));
  asm volatile("vse32.v  v2, (%0); add %0, %0, %1" : "+&r"(o_) : "r"(ldo));

  asm volatile("vslidedown.vi v28, v16,  2");
  asm volatile("vmacc.vx v4, %0, v26" ::"r"(t7));
  asm volatile("vmacc.vx v6, %0, v26" ::"r"(t6));

  asm volatile("vmacc.vx v4, %0, v28" ::"r"(t8));

  asm volatile("vslidedown.vi v30, v18,  2");
  asm volatile("vse32.v  v4, (%0); add %0, %0, %1" : "+&r"(o_) : "r"(ldo));
  asm volatile("vmacc.vx v6, %0, v28" ::"r"(t7));

  asm volatile("vmacc.vx v6, %0, v30" ::"r"(t8));
  asm volatile("vse32.v  v6, (%0); add %0, %0, %1" : "+&r"(o_) : "r"(ldo));
  }
}


