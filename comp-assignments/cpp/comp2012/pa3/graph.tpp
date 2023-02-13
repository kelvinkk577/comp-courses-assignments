// TODO: Implement the member functions of Node (Part (B), except the output operator)
//             and the graph operations (Part (C))
template <typename T>
Node<T>::Node(const T &val)
{
  this->val = val;
  out = nullptr;
  capacity = 0;
  size_p = 0;
}

template <typename T>
Node<T>::Node(const Node<T> &that)
{
  this->val = that.val;
  out = nullptr;
  capacity = 0;
  size_p = 0;
}

template <typename T>
Node<T>::~Node()
{
  delete[] out;
}

template <typename T>
T &Node<T>::operator*()
{
  return val;
}

template <typename T>
const T &Node<T>::operator*() const
{
  return val;
}

template <typename T>
unsigned int Node<T>::degree() const
{
  int degree = 0;
  for (int i = 0; i < size_p; i++)
    if (!out[i].is_null())
      degree += 1;
  return degree;
}

template <typename T>
unsigned int Node<T>::size() const
{
  return size_p;
}

template <typename T>
SmartPtr<Node<T>> Node<T>::operator[](unsigned int i) const
{
  if (out[i].is_null())
  {
    return SmartPtr<Node<T>>();
  }
  return out[i];
}

template <typename T>
void Node<T>::add(SmartPtr<Node<T>> n)
{
  if (n.is_null())
    return;
  for (int i = 0; i < size_p; i++)
  {
    if (out[i].is_null())
      continue;

    if (out[i] == n)
      return;
  }

  // Allocate memory for the array when space is needed
  if (capacity == 0 || size_p >= capacity)
  {
    int size = 2 * capacity;
    if (init_capacity > size)
      size = init_capacity;

    // Copy old array content to expanded array
    SmartPtr<Node<T>> *temp = new SmartPtr<Node<T>>[size];
    for (int i = 0; i < size_p; i++)
      temp[i] = out[i];
    capacity = size;

    delete[] out;
    out = temp;
  }

  out[size_p++] = n;
}

template <typename T>
void Node<T>::remove(SmartPtr<Node<T>> n)
{
  if (n.is_null())
    return;
  for (int i = 0; i < size_p; i++)
  {
    if (out[i].is_null())
      continue;

    if (out[i] == n)
    {
      out[i].unset();
      return;
    }
  }
}

template <typename T>
bool Node<T>::exists(SmartPtr<Node<T>> n) const
{
  if (!n.is_null())
  {
    for (int i = 0; i < size_p; i++)
    {
      if (!out[i].is_null())
        if (out[i] == n)
          return true;
    }
  }
  return false;
}

template <typename T>
SmartPtr<Node<T>> Node<T>::find(T val) const
{
  for (int i = 0; i < size_p; i++)
  {
    if (!out[i].is_null())
    {
      if (**out[i] == val)
        return out[i];
    }
  }
  return SmartPtr<Node<T>>();
}

template <typename T>
SmartPtr<Node<T>> new_node(const T &val)
{
  return SmartPtr<Node<T>>(Node<T>(val));
}

template <typename T>
void remove_node(SmartPtr<Node<T>> n)
{
  if (n.is_null())
    return;

  for (int i = 0; i < (*n).size(); i++)
  {
    if ((*n)[i].is_null())
      continue;

    (*((*n)[i])).remove(n);
    (*n).remove((*n)[i]);
  }
}

template <typename T>
void add_edge(SmartPtr<Node<T>> n1, SmartPtr<Node<T>> n2)
{
  if (n1.is_null() || n2.is_null())
    return;

  if ((*n1).exists(n2))
    return;

  if (n1 == n2)
    return;

  (*n1).add(n2);
  (*n2).add(n1);
}

template <typename T>
void remove_edge(SmartPtr<Node<T>> n1, SmartPtr<Node<T>> n2)
{
  if (n1.is_null() || n2.is_null())
    return;

  if (!((*n1).exists(n2)))
    return;

  (*n1).remove(n2);
  (*n2).remove(n1);
}

template <typename T>
void remove_graph(SmartPtr<Node<T>> root)
{
  if (root.is_null())
    return;

  set<Node<T> *> visited;
  queue<SmartPtr<Node<T>>> tempQueue;
  queue<SmartPtr<Node<T>>> allNodes;

  visited.insert(root.operator->());
  tempQueue.push(root);

  while (!tempQueue.empty())
  {
    SmartPtr<Node<T>> front = tempQueue.front();
    tempQueue.pop();
    allNodes.push(front);
    for (int i = 0; i < (*front).size(); i++)
    {
      SmartPtr<Node<T>> neighbor = (*front)[i];

      if (neighbor.is_null())
        continue;

      if (visited.count(neighbor.operator->()) != 1)
      {
        visited.insert(neighbor.operator->());
        tempQueue.push(neighbor);
      }
    }
  }
  while (allNodes.size() > 0)
  {
    remove_node(allNodes.front());
    allNodes.pop();
  }
}

template <typename T>
SmartPtr<Node<T>> find(SmartPtr<Node<T>> root, T val)
{
  if (root.is_null())
    return SmartPtr<Node<T>>();

  set<Node<T> *> visited;
  queue<SmartPtr<Node<T>>> tempQueue;
  queue<SmartPtr<Node<T>>> allNodes;

  visited.insert(root.operator->());
  tempQueue.push(root);

  while (!tempQueue.empty())
  {
    SmartPtr<Node<T>> front = tempQueue.front();
    tempQueue.pop();
    allNodes.push(front);
    for (int i = 0; i < (*front).size(); i++)
    {
      SmartPtr<Node<T>> neighbor = (*front)[i];

      if (neighbor.is_null())
        continue;

      if (visited.count(neighbor.operator->()) != 1)
      {
        visited.insert(neighbor.operator->());
        tempQueue.push(neighbor);
      }
    }
  }
  while (allNodes.size() > 0)
  {
    if (**allNodes.front() == val)
      return allNodes.front();
    allNodes.pop();
  }
  return SmartPtr<Node<T>>();
}

template <typename T>
bool reachable(SmartPtr<Node<T>> root, SmartPtr<Node<T>> dest)
{
  if (root.is_null() || dest.is_null())
    return false;

  set<Node<T> *> visited;
  queue<SmartPtr<Node<T>>> tempQueue;

  visited.insert(root.operator->());
  tempQueue.push(root);

  while (!tempQueue.empty())
  {
    SmartPtr<Node<T>> front = tempQueue.front();
    tempQueue.pop();

    for (int i = 0; i < (*front).size(); i++)
    {
      SmartPtr<Node<T>> neighbor = (*front)[i];

      if (neighbor.is_null())
        continue;

      if (neighbor == dest)
        return true;

      if (visited.count(neighbor.operator->()) != 1)
      {
        visited.insert(neighbor.operator->());
        tempQueue.push(neighbor);
      }
    }
  }
  return false;
}