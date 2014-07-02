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

// Tests PString().

// Tests PString for its correctness.
TEST(PStringUnitTest, Init)
{
  PString str;
  EXPECT_EQ(str.size_, 0U);
  EXPECT_EQ(nullptr, str.data_);
}

TEST(PStringUnitTest, InitWithParam1)
{
  PString str("ABCDEF");
  EXPECT_EQ(str.size_, 7U);
  EXPECT_STREQ(str.data_, "ABCDEF");
}

TEST(PStringUnitTest, InitWithParam2)
{
  PString str("ABCDEFG");
  EXPECT_EQ(str.size_, 8U);
  EXPECT_STREQ(str.data_, "ABCDEFG");
}

TEST(PStringUnitTest, InitWithParam3)
{
  PString str("ABCDEFGH");
  EXPECT_EQ(str.size_, 9U);
  EXPECT_STREQ(str.data_, "ABCDEFGH");
}