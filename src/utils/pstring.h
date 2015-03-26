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

#ifndef POOLED_STRING_H
#define POOLED_STRING_H

#include "charpool.h"
#include <string>
#include <iostream>

#ifndef STRING_TYPE
#define STRING_TYPE PString
//#define STRING_TYPE std::string 
#endif //STRING_TYPE

//#define USE_POOL

#define DEFAULT_CHAR_ARR_SIZE_MINUS_ONE (DEFAULT_CHAR_ARR_SIZE - 1)

struct PString
{
private:
#ifdef USE_POOL
  static CharPool<> pool_;
#endif //USE_POOL
  size_t size_;
  char *data_;
  size_t *ptr_count_;

  inline static size_t getNumCells(int sz)
  {
    return (sz & DEFAULT_CHAR_ARR_SIZE_MINUS_ONE ? 
            (sz / DEFAULT_CHAR_ARR_SIZE) + 1 :
            (sz / DEFAULT_CHAR_ARR_SIZE)) ;
  }

protected:
  //friends
  friend bool operator==(const char *, const PString &);
  friend bool operator!=(const char *, const PString &);
  friend std::ostream &operator<< (std::ostream &o, PString const &str);
  friend size_t hash_value(PString const &str);

public:
  PString() : size_(0), data_(nullptr), ptr_count_(new size_t(1))
  {
  }

  PString(const char *str) : ptr_count_(new size_t(1))
  {
    size_ = strlen(str) + 1;
#ifdef USE_POOL
    size_t num_cells = getNumCells(size_);
    data_ = pool_.add(num_cells);
#else
    data_ = new char[size_];
#endif //USE_POOL
    memcpy(data_, str, size_ * sizeof(char));
  }

  PString(const char *str, size_t strln) : ptr_count_(new size_t(1))
  {
    size_ = strln + 1;
#ifdef USE_POOL
    size_t num_cells = getNumCells(size_);
    data_ = pool_.add(num_cells);
#else
    data_ = new char[size_];
#endif //USE_POOL
    memcpy(data_, str, strln * sizeof(char));
    data_[strln] = '\0';
  }

//   PString(const std::string &str) : ptr_count_(new size_t(1))
//   {
//     size_ = str.length() + 1;
// #ifdef USE_POOL
//     size_t num_cells = getNumCells(size_);
//     data_ = pool_.add(num_cells);
// #else
//     data_ = new char[size_];
// #endif //USE_POOL
//     memcpy(data_, str.c_str(), size_ * sizeof(char));
//   }

  PString(const PString &pstr)
  {
    *pstr.ptr_count_ += 1;
    this->ptr_count_ = pstr.ptr_count_;
    this->data_ = pstr.data_;
    this->size_ = pstr.size_;
  }

  ~PString()
  {
    if ((--(*ptr_count_)) == 0)
    {
      delete ptr_count_;
      ptr_count_ = nullptr;
      if (data_) 
	    {
#ifdef USE_POOL
        pool_.del(getNumCells(size_), data_);
#else
        delete[] data_;
#endif //USE_POOL
        data_ = nullptr;
      }
    }
  }

  FORCE_INLINE char *c_str()
  {
    return data_;
  }

  FORCE_INLINE const char *c_str() const
  {
    return data_;
  }

  inline char &operator[](const int x)
  {
    return data_[x];
  }

  inline const char &operator[](const int x) const
  {
    return data_[x];
  }

  PString &operator=(const char *str)
  {
#ifdef USE_POOL
    if ((--(*ptr_count_)) == 0)
    {
      (*ptr_count_) = 1;  // re-use ptr_count
      if (data_) pool_.del(getNumCells(size_), data_);
    }
    else ptr_count_ = new size_t(1);
    size_ = strlen(str) + 1;
    size_t num_cells = getNumCells(size_);
    data_ = pool_.add(num_cells);
#else
    if ((--(*ptr_count_)) == 0)
    {
      (*ptr_count_) = 1;  // re-use ptr_count
      if (data_) delete[] data_;
    }
    else ptr_count_ = new size_t(1);
    size_t sz = strlen(str) + 1;
    size_ = sz;
    data_ = new char[size_];
#endif //USE_POOL
    memcpy(data_, str, size_ * sizeof(char));
    return *this;
  }

  PString &operator=(const PString &pstr)
  {
    if ((--(*ptr_count_)) == 0) 
    {
      delete ptr_count_;
      if (data_)
      {
#ifdef USE_POOL
        pool_.del(getNumCells(size_), data_);
#else
        delete[] data_;
#endif //USE_POOL
      }
    }
    (*pstr.ptr_count_)++;
    this->ptr_count_ = pstr.ptr_count_;
    this->data_ = pstr.data_;
    this->size_ = pstr.size_;
    return *this;
  }

  size_t length() const
  {
    return size_ - 1;
  }

  size_t maxsize() const
  {
    return getNumCells(size_) * DEFAULT_CHAR_ARR_SIZE - 1;
  }

  int end() const
  {
    return size_ - 1;
  }

  inline bool operator==(const PString &other) const
  {
    return (this->size_ == other.size_ &&
            strcmp(this->data_, other.data_) == 0);
  }

  inline bool operator!=(const PString &other) const
  {
    return (this->size_ != other.size_ || 
            strcmp(this->data_, other.data_) != 0); 
  }

  inline bool operator<(const PString &other) const
  {
    if (this->size_ != other.size_)
    {
      return (this->size_ < other.size_);
    }
    char *tmp1 = this->data_;
    char *tmp2 = other.data_;
    while (*tmp1 != 0)
    {
      if (*tmp1 != *tmp2)
      {
        return (*tmp1 < *tmp2);
      }
      ++tmp1;
      ++tmp2;
    }
    return false;
  }

  inline bool operator==(const char *other)
  {
    return (strcmp(this->data_, other) == 0);
  }

  inline bool operator!=(const char *other)
  {
    return (strcmp(this->data_, other) != 0);
  }

  PString substr (size_t pos) const
  {
    return PString(this->data_ + pos);
  }

  PString substr (size_t pos, size_t len) const
  {
    return PString(this->data_ + pos, len);
  }
};

#endif //POOLED_STRING_H
