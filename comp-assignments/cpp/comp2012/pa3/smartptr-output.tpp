// TODO: Implement the output operator<< for SmartPtr
template <typename T>
ostream &operator<<(ostream &os, const SmartPtr<T> &sp)
{
  if (sp.is_null())
    return (os << "SmartPtr()");

  return (os << "SmartPtr(" << *sp.ptr << "," << *sp.count << ")");
}