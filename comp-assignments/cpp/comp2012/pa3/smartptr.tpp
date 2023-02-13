// TODO: Implement the member functions of SmartPtr (Part (A), except the output operator)
template <typename T>
SmartPtr<T>::SmartPtr()
{
  ptr = nullptr;
  count = nullptr;
};

template <typename T>
SmartPtr<T>::SmartPtr(const T &val)
{
  ptr = new T(val);
  count = new unsigned int(1);
}

template <typename T>
SmartPtr<T>::SmartPtr(const SmartPtr<T> &that)
{
  ptr = that.ptr;
  count = that.count;
  *count += 1;
}

template <typename T>
SmartPtr<T>::~SmartPtr()
{
  if (count)
  {
    *count -= 1;
    if (*count == 0)
    {
      delete ptr;
      delete count;
    }
  }
}

template <typename T>
SmartPtr<T> &SmartPtr<T>::operator=(const SmartPtr<T> &that)
{
  if (this != &that)
  {
    if (count)
    {
      *count -= 1;
      if (*count == 0)
      {
        delete ptr;
        delete count;
      }
    }

    ptr = that.ptr;
    count = that.count;
    if (!that.is_null())
      *count += 1;
  }

  return *this;
}

template <typename T>
void SmartPtr<T>::set(const T &val)
{
  if (count)
  {
    *count -= 1;
    if (*count == 0)
    {
      delete ptr;
      delete count;
    }
  }

  ptr = new T(val);
  count = new unsigned int(1);
}

template <typename T>
void SmartPtr<T>::unset()
{
  if (count)
  {
    *count -= 1;
    if (*count == 0)
    {
      delete ptr;
      delete count;
    }
  }

  ptr = nullptr;
  count = nullptr;
}

template <typename T>
bool SmartPtr<T>::is_null() const
{
  return (ptr == nullptr);
}

template <typename T>
bool SmartPtr<T>::operator==(const SmartPtr<T> &that) const
{
  return (ptr == that.ptr);
}

template <typename T>
bool SmartPtr<T>::operator!=(const SmartPtr<T> &that) const
{
  return (ptr != that.ptr);
}

template <typename T>
T &SmartPtr<T>::operator*() const
{
  return *ptr;
}

template <typename T>
T *SmartPtr<T>::operator->() const
{
  return ptr;
}