#include "pstring.h"

CharPool<> PString::pool_;

//global operators
bool operator==(const PString &str1, const PString &str2)
{
  if (str1.size_ != str2.size_)
  {
    return false;
  }
  char *tmp1 = str1.data_;
  char *tmp2 = str2.data_;
  while (*tmp1 != 0)
  {
    if (*tmp1 != *tmp2)
    {
      return false;
    }
    ++tmp1;
    ++tmp2;
  }
  return true;
}

bool operator==(const PString &str1, const char *str2)
{
  size_t size = strlen(str2) + 1;
  if (str1.size_ != size)
  {
    return false;
  }
  char *tmp = str1.data_;
  while (*tmp != 0)
  {
    if (*tmp != *str2)
    {
      return false;
    }
    ++tmp;
    ++str2;
  }
  return true;
}

bool operator==(const char *str1, const PString &str2)
{
  size_t size = strlen(str1) + 1;
  if (str2.size_ != size)
  {
    return false;
  }
  char *tmp = str2.data_;
  while (*tmp != 0)
  {
    if (*tmp != *str1)
    {
      return false;
    }
    ++tmp;
    ++str1;
  }
  return true;
}

bool operator!=(const PString &str1, const PString &str2)
{
  return !(str1 == str2);
}

bool operator!=(const PString &str1, const char *str2)
{
  return !(str1 == str2);
}

bool operator!=(const char *str1, const PString &str2)
{
  return !(str1 == str2);
}