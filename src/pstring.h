#ifndef POOLED_STRING_H
#define POOLED_STRING_H

#include "charpool.h"
#include <string>

#define DEFAULT_CHAR_ARR_SIZE_MINUS_ONE (DEFAULT_CHAR_ARR_SIZE - 1)

struct PString
{
private:
  static CharPool<> pool_;
  inline static size_t getNumCells(int sz)
  {
    size_t num_cells = sz / DEFAULT_CHAR_ARR_SIZE;
    // if(num_cells * DEFAULT_CHAR_ARR_SIZE != sz) num_cells++;
    if (sz & DEFAULT_CHAR_ARR_SIZE_MINUS_ONE) num_cells++;
    return num_cells;
  }
protected:
  //friends
  friend bool operator==(const PString &, const PString &);
  friend bool operator==(const PString &, const char *);
  friend bool operator==(const char *, const PString &);
public:
  size_t size_;
  char *data_;

  PString() : size_(0), data_(nullptr) {}
  PString(const char *str)
  {
    size_ = strlen(str) + 1;
    size_t num_cells = getNumCells(size_);
    data_ = pool_.add(num_cells);
    memcpy(data_, str, size_ * sizeof(char));
  }
  PString(const std::string &str)
  {
    size_ = str.length() + 1;
    size_t num_cells = getNumCells(size_);
    data_ = pool_.add(num_cells);
    memcpy(data_, str.c_str(), size_ * sizeof(char));
  }
  ~PString()
  {
    if (data_) pool_.del(getNumCells(size_), data_);
  }
  inline operator char *()
  {
    return data_;
  }
  inline operator const char *() const
  {
    return data_;
  }
  inline char *c_str()
  {
    return data_;
  }
  inline const char *c_str() const
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
    size_t sz = strlen(str) + 1;
    size_t num_cells = getNumCells(size_);
    size_t max_capacity = (num_cells * DEFAULT_CHAR_ARR_SIZE);
    if (sz < max_capacity && sz > max_capacity - DEFAULT_CHAR_ARR_SIZE)
    {
      memcpy(data_, str, sz);
    }
    else
    {
      pool_.del(num_cells, data_);
      size_ = sz;
      num_cells = getNumCells(size_);
      data_ = pool_.add(num_cells);
      memcpy(data_, str, size_ * sizeof(char));
    }
    return *this;
  }
  PString &operator=(const PString &pstr)
  {
    char *str = pstr.data_;
    size_t sz = pstr.size_;
    size_t num_cells = getNumCells(size_);
    size_t max_capacity = (num_cells * DEFAULT_CHAR_ARR_SIZE);
    if (sz < max_capacity && sz > max_capacity - DEFAULT_CHAR_ARR_SIZE)
    {
      memcpy(data_, str, sz);
    }
    else
    {
      pool_.del(num_cells, data_);
      size_ = sz;
      num_cells = getNumCells(size_);
      data_ = pool_.add(num_cells);
      memcpy(data_, str, size_ * sizeof(char));
    }
    return *this;
  }
  void clear()
  {
    if (!data_) return;

    if (size_ <= DEFAULT_CHAR_ARR_SIZE) data_[0] = '\0';
    else
    {
      pool_.del(getNumCells(size_), data_);
      data_ = nullptr;
      size_ = 0;
    }
  }
  size_t length() const
  {
    return size_ - 1;
  }
  size_t maxsize() const
  {
    return getNumCells(size_)*DEFAULT_CHAR_ARR_SIZE - 1;
  }
  int end() const
  {
    return size_ - 1;
  }
};

#endif //POOLED_STRING_H