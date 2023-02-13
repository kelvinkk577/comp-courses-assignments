// TODO: Implement the output operator<< for Node
template <typename T>
ostream &operator<<(ostream &os, const Node<T> &n)
{
  os << "Node(" << *n << ",{";
  int maxComma = n.degree() - 1;
  int commaCount = 0;
  for (int i = 0; i < n.size(); i++)
  {
    if (!(n[i].is_null()))
    {
      os << **n[i];
      if (commaCount < maxComma)
      {
        os << ",";
        commaCount++;
      }
    }
  }

  os << "})";
  return os;
}