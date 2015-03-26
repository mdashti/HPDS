// The MIT License (MIT)

// Copyright (c) 2014 Mohammad Dashti
// (www.mdashti.com - mohammad.dashti [at] epfl [dot] ch - mdashti [at] gmail [dot] com)

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <limits.h>
#undef private
#define private public
#include "../src/utils/charpool.h"
#undef private
#define private private
#include <gtest/gtest.h>


typedef union __El
{
  __El() {}; ~__El() {};
  // char obj[char_arr_size];
  union __El *next;
} El;

size_t  sizeof_data(void* data_, size_t size_) {
  size_t res = 0U;
  size_t sz = size_;
  El* tmp = static_cast<El*>(data_);
  while (tmp)
  {
    res += sz;
    El *el = tmp[sz].next;
    tmp = el;
    sz = sz >> 1;
  }
  return res;
}

size_t  sizeof_free(void* free_) {
  size_t res = 0U;
  El* tmp = static_cast<El*>(free_);
  while (tmp)
  {
    res += 1;
    tmp = tmp->next;
  }
  return res;
}

// Tests CharPool().

// Tests Pool for its correctness.
TEST(CharPoolUnitTest, Init)
{
  CharPool<> pool;
  EXPECT_EQ(pool.size_, CHARPOOL_DEFAULT_CHUNK_SIZE);
  EXPECT_EQ(pool.forceClear_, false);
  EXPECT_EQ(pool.num_cell_left_in_index0_, CHARPOOL_DEFAULT_CHUNK_SIZE);
  EXPECT_EQ(sizeof pool.free_ / sizeof(int*), DEFAULT_NUM_RESERVED_CELLS+1);
  EXPECT_EQ(pool.free_[0], pool.data_);
  EXPECT_EQ(sizeof_data(pool.data_,pool.size_), CHARPOOL_DEFAULT_CHUNK_SIZE);
}

TEST(CharPoolUnitTest, InitWithParam1)
{
  const size_t chunk_size = 16;
  const size_t chunks_added = 15;
  CharPool<> pool(chunk_size);
  pool.add(chunks_added);
  EXPECT_EQ(pool.size_, chunk_size);
  EXPECT_EQ(pool.forceClear_, false);
  EXPECT_EQ(pool.num_cell_left_in_index0_, chunk_size-chunks_added);
  EXPECT_EQ(sizeof pool.free_ / sizeof(int*), DEFAULT_NUM_RESERVED_CELLS+1);
}

TEST(CharPoolUnitTest, InitWithParam2)
{
  const size_t chunk_size = 16;
  const size_t chunks_added = 16;
  CharPool<> pool(chunk_size);
  pool.add(chunks_added);
  EXPECT_EQ(pool.size_, chunk_size);
  EXPECT_EQ(pool.forceClear_, false);
  EXPECT_EQ(pool.num_cell_left_in_index0_, chunk_size-chunks_added);
  EXPECT_EQ(sizeof pool.free_ / sizeof(int*), DEFAULT_NUM_RESERVED_CELLS+1);
}

TEST(CharPoolUnitTest, InitWithParam3)
{
  const size_t chunk_size = 16;
  const size_t chunks_added = 17;
  CharPool<> pool(chunk_size);
  pool.add(chunks_added);
  EXPECT_EQ(pool.size_, chunk_size * 2);
  EXPECT_EQ(pool.forceClear_, true);
  EXPECT_EQ(pool.num_cell_left_in_index0_, (2 * chunk_size)-chunks_added);
  EXPECT_EQ(sizeof_free(pool.free_[1]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[2]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[3]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[4]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[5]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[16]), 1U);
  EXPECT_EQ(sizeof pool.free_ / sizeof(int*), DEFAULT_NUM_RESERVED_CELLS+1);
  EXPECT_EQ(sizeof_data(pool.data_,pool.size_), chunk_size * 2 + chunk_size);

  pool.add(16);
  EXPECT_EQ(pool.size_, chunk_size * 2);
  EXPECT_EQ(pool.forceClear_, true);
  EXPECT_EQ(pool.num_cell_left_in_index0_, (2 * chunk_size)-chunks_added);
  EXPECT_EQ(sizeof_free(pool.free_[1]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[2]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[3]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[4]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[5]), 0U);
  EXPECT_EQ(sizeof_free(pool.free_[16]), 0U);
  EXPECT_EQ(sizeof pool.free_ / sizeof(int*), DEFAULT_NUM_RESERVED_CELLS+1);
  EXPECT_EQ(sizeof_data(pool.data_,pool.size_), chunk_size * 2 + chunk_size);

}

TEST(CharPoolUnitTest, InitWithParam4)
{
  const size_t chunk_size = 16;
  const size_t chunks_added = DEFAULT_NUM_RESERVED_CELLS+1;
  CharPool<> pool(chunk_size);
  pool.add(chunks_added);

  EXPECT_EQ(pool.size_, chunk_size);
  EXPECT_EQ(pool.forceClear_, false);
  EXPECT_EQ(pool.num_cell_left_in_index0_, chunk_size);
  EXPECT_EQ(sizeof pool.free_ / sizeof(int*), DEFAULT_NUM_RESERVED_CELLS+1);
  EXPECT_EQ(pool.free_[0], pool.data_);
  EXPECT_EQ(sizeof_data(pool.data_,pool.size_), chunk_size);
}
