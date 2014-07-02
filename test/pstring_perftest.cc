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
#define private public
#include "../src/pstring.h"
#define private private
#include <gtest/gtest.h>

#define DEFAULT_NUM_ITER 1000000
#define MAX_LEN (DEFAULT_CHAR_ARR_SIZE * (DEFAULT_NUM_RESERVED_CELLS + 1))

void gen_random(char *s, const int max_len)
{
  const int len = rand() % max_len;

  static const char alphanum[] =
    "0123456789 "
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
    "abcdefghijklmnopqrstuvwxyz ";

  for (int i = 0; i < len; ++i)
  {
    s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  s[len] = 0;
}

// Tests PString().

// Tests PString for its performance.
TEST(PStringPerfTest, TestPString)
{
  srand(123456);
  const int num = DEFAULT_NUM_ITER;
  PString **p = new PString *[num];
  char tmp[MAX_LEN];
  for (int i = 0; i < num; ++i)
  {
    gen_random(tmp, MAX_LEN);
    p[i] = new PString(tmp);
  }
}

TEST(PStringPerfTest, TestCppString)
{
  srand(123456);
  const int num = DEFAULT_NUM_ITER;
  std::string **p = new std::string *[num];
  char tmp[MAX_LEN];
  for (int i = 0; i < num; ++i)
  {
    gen_random(tmp, MAX_LEN);
    p[i] = new std::string(tmp);
  }
}

TEST(PStringPerfTest, TestCharStar)
{
  srand(123456);
  const int num = DEFAULT_NUM_ITER;
  char **p = new char *[num];
  char tmp[MAX_LEN];
  for (int i = 0; i < num; ++i)
  {
    gen_random(tmp, MAX_LEN);
    size_t sz = strlen(tmp) + 1;
    p[i] = new char[sz];
    memcpy(p[i], tmp, sz * sizeof(char));
  }
}