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
#include "../src/utils/pool.h"
#include <gtest/gtest.h>

#define DEFAULT_NUM_ITER 50000000

// Tests Pool().

struct rec1
{
  int _1;
  double _3;
  int _2;
  int _val;
  rec1 *nxt;
  rec1 *prv;
};

struct rec2
{
  rec2(){}
  int _1;
  double _3;
  int _2;
  int _val;
  rec2 *nxt;
  rec2 *prv;
};

struct rec3
{
  rec3(): _1(0), _3(0.0), _2(0), _val(0), nxt(nullptr), prv(nullptr) {}
  rec3(int _1, int _2 = 0, double _3 = 0.0, int _val = 0) : nxt(nullptr), prv(nullptr)
  {
    this->_1 = _1;
    this->_2 = _2;
    this->_3 = _3;
    this->_val = _val;
  }
  int _1;
  double _3;
  int _2;
  int _val;
  rec3 *nxt;
  rec3 *prv;
};

TEST(PoolPerfTest, Rec1Pool)
{
  const int num = DEFAULT_NUM_ITER;
  Pool<rec1> pool(num);
  rec1 **p = new rec1 *[num];
  for (int i = 0; i < num; ++i)
  {
    p[i] = pool.add();
  }
}

TEST(PoolPerfTest, Rec1NoPool)
{
  const int num = DEFAULT_NUM_ITER;
  Pool<rec1> pool(num);
  rec1 **p = new rec1 *[num];
  for (int i = 0; i < num; ++i)
  {
    p[i] = new rec1;
  }
}

TEST(PoolPerfTest, Rec2Pool)
{
  const int num = DEFAULT_NUM_ITER;
  Pool<rec2> pool(num);
  rec2 **p = new rec2 *[num];
  for (int i = 0; i < num; ++i)
  {
    p[i] = pool.add();
  }
}

TEST(PoolPerfTest, Rec2NoPool)
{
  const int num = DEFAULT_NUM_ITER;
  Pool<rec2> pool(num);
  rec2 **p = new rec2 *[num];
  for (int i = 0; i < num; ++i)
  {
    p[i] = new rec2;
  }
}

TEST(PoolPerfTest, Rec3Pool)
{
  const int num = DEFAULT_NUM_ITER;
  Pool<rec3> pool(num);
  rec3 **p = new rec3 *[num];
  for (int i = 0; i < num; ++i)
  {
    p[i] = pool.add();
  }
}

TEST(PoolPerfTest, Rec3NoPool)
{
  const int num = DEFAULT_NUM_ITER;
  Pool<rec3> pool(num);
  rec3 **p = new rec3 *[num];
  for (int i = 0; i < num; ++i)
  {
    p[i] = new rec3;
  }
}

TEST(PoolPerfTest, Rec3WithCtorPool)
{
  const int num = DEFAULT_NUM_ITER;
  Pool<rec3> pool(num);
  rec3 **p = new rec3 *[num];
  for (int i = 0; i < num; ++i)
  {
    p[i] = pool.add();
    new(p[i]) rec3(i,i+1,i+2,i+3);
  }
}

TEST(PoolPerfTest, Rec3WithCtorNoPool)
{
  const int num = DEFAULT_NUM_ITER;
  Pool<rec3> pool(num);
  rec3 **p = new rec3 *[num];
  for (int i = 0; i < num; ++i)
  {
    p[i] = new rec3(i,i+1,i+2,i+3);
  }
}
