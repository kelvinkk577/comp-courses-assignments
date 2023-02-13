#include <iostream>
#include <string>
#include "smartptr.h"
#include "graph.h"

using namespace std;

// Default constructor
void t1()
{
  SmartPtr<int> ptr;
  ptr.print();
}

// Conversion constructor
void t2()
{
  SmartPtr<int> ptr(5);
  ptr.print();
}

// Copy constructor
void t3()
{
  SmartPtr<int> ptr(5);
  ptr.print();
  SmartPtr<int> ptr_copy(ptr);
  ptr.print();
  ptr_copy.print();
}

// Assigment operator and cascading
void t4()
{
  SmartPtr<int> ptr;
  SmartPtr<int> ptr2(3);
  SmartPtr<int> ptr3(7);
  ptr.print();
  ptr2.print();
  ptr3.print();

  ptr = ptr2 = ptr3;

  ptr.print();
  ptr2.print();
  ptr3.print();
}

// Member function: set
void t5()
{
  SmartPtr<int> ptr(5);
  SmartPtr<int> ptr2(ptr);

  ptr.print();
  ptr2.print();

  ptr.set(10);
  ptr.print();
  ptr2.print();
}

// Member function: unset
void t6()
{
  SmartPtr<int> ptr(5);
  SmartPtr<int> ptr2(ptr);

  ptr.print();
  ptr2.print();

  ptr.unset();
  ptr.print();
  ptr2.print();
}

// Member function: is_null
void t7()
{
  SmartPtr<int> ptr;
  cout << boolalpha << ptr.is_null() << endl;
  ptr.set(5);
  cout << boolalpha << ptr.is_null() << endl;

  // SmartPtr<int> *ptr2 = nullptr;
  // cout << boolalpha << ptr2->is_null() << endl;
}

// ==, !=
void t8()
{
  SmartPtr<int> ptr;
  SmartPtr<int> ptr2(3);
  SmartPtr<int> ptr3;
  cout << boolalpha << (ptr != ptr2) << endl;
}

// *ptr
void t9()
{
  SmartPtr<int> ptr;
  SmartPtr<int> ptr2(5);
  cout << *ptr << endl; // Error
  cout << *ptr2 << endl;
}

// ptr->
void t10()
{
  SmartPtr<int> ptr;
  SmartPtr<int> ptr2(5);
  // cout << << endl;
  // cout < < < < endl;
}

// operator<<
void t11()
{
  SmartPtr<int> ptr;
  SmartPtr<int> ptr2(5);
  SmartPtr<int> ptr3(ptr2);
  SmartPtr<string> ptr4("Hello World");
  SmartPtr<string> ptr5("abc");
  ptr5 = ptr4;

  cout << ptr << ", " << ptr2 << ", " << ptr3 << ", " << ptr4 << ", " << endl;
}

void n1()
{
  Node<int> node(5);
  node.print();
  Node<int> node2(node);
  node2.print();

  node.print();
  node2.print();
}

void n2()
{
  Node<int> node(2);
  int val = *node;
  val = 5;
  *node = 3;
  cout << "local variable: " << val << endl;
  node.print();
}

void n3()
{
  Node<int> node(2);
  node.add(SmartPtr<Node<int>>(5));
}

void n4()
{
  Node<int> node(2);
  node.print();

  Node<int> node2(3);
  Node<int> node3(3);
  Node<int> node4(4);

  SmartPtr<Node<int>> ptr(node2);
  SmartPtr<Node<int>> ptr2(node3);
  SmartPtr<Node<int>> ptr3(node4);

  node.add(ptr);
  node.add(ptr2);
  node.add(ptr3);
  node.add(node2);
  node.add(node2);
  node.add(node2);
  node.add(node2);
  node.print();
}

void n5()
{
  Node<int> node(2);
  node.print();

  Node<int> node2(3);
  Node<int> node3(3);
  Node<int> node4(4);

  SmartPtr<Node<int>> ptr(node2);
  SmartPtr<Node<int>> ptr2(node3);
  SmartPtr<Node<int>> ptr3(node4);

  node.add(ptr);
  node.add(ptr2);
  node.add(ptr3);

  node.remove(ptr);
  node.print();

  node.add(ptr2);
  node.add(node2);
  // node.add(node2);
  node.print();
}

// exist
void n6()
{
  Node<int> node(2);
  node.print();

  Node<int> node2(3);
  Node<int> node3(3);
  Node<int> node4(4);

  SmartPtr<Node<int>> ptr(node2);
  SmartPtr<Node<int>> ptr2(node3);
  SmartPtr<Node<int>> ptr3(node4);

  node.add(ptr);
  node.add(ptr2);

  cout << boolalpha << node.exists(ptr3) << endl;

  node.add(ptr3);
  cout << boolalpha << node.exists(ptr3) << endl;
}

// find
void n7()
{
  Node<int> node(2);
  node.print();

  Node<int> node2(3);
  Node<int> node3(3);
  Node<int> node4(4);

  SmartPtr<Node<int>> ptr(node2);
  SmartPtr<Node<int>> ptr2(node3);
  SmartPtr<Node<int>> ptr3(node4);

  node.add(ptr);
  node.add(ptr2);
  node.add(ptr3);

  node.print();

  node.find(3).print();

  node.remove(ptr);

  node.find(3).print();
  node.remove(ptr);

  node.find(3).print();
}

void n8()
{
  Node<int> node(2);
  node.print();

  Node<int> node2(3);
  Node<int> node3(3);
  Node<int> node4(4);

  SmartPtr<Node<int>> ptr(node2);
  SmartPtr<Node<int>> ptr2(node3);
  SmartPtr<Node<int>> ptr3(node4);

  node.add(ptr);
  node.add(ptr2);
  node.add(ptr3);

  cout << node;

  node.remove(ptr3);
}

void n9()
{
  SmartPtr<Node<int>> n1 = new_node(1);
  SmartPtr<Node<int>> n2 = new_node(1);
  SmartPtr<Node<int>> n3 = new_node(1);
  SmartPtr<Node<int>> n4 = new_node(1);
  SmartPtr<Node<int>> n5 = new_node(1);
  SmartPtr<Node<int>> n6 = new_node(1);

  add_edge(n1, n2);
  add_edge(n1, n5);
  add_edge(n5, n6);
  add_edge(n2, n4);
  add_edge(n3, n4);

  cout << "n1: " << n1 << endl;
  cout << "n2: " << n2 << endl;
  cout << "n3: " << n3 << endl;
  cout << "n4: " << n4 << endl;
  cout << "n5: " << n5 << endl;
  cout << "n6: " << n6 << endl;

  remove_graph(n5);
  cout << endl;

  cout << "n1: " << n1 << endl;
  cout << "n2: " << n2 << endl;
  cout << "n3: " << n3 << endl;
  cout << "n4: " << n4 << endl;
  cout << "n5: " << n5 << endl;
  cout << "n6: " << n6 << endl;
}

void n10()
{
  SmartPtr<Node<int>> n1 = new_node(1);
  SmartPtr<Node<int>> n2 = new_node(2);
  SmartPtr<Node<int>> n3 = new_node(3);
  SmartPtr<Node<int>> n4 = new_node(4);

  add_edge(n1, n2);
  add_edge(n1, n4);
  add_edge(n2, n3);
  add_edge(n3, n4);

  cout << "n1: " << n1 << endl;
  cout << "n2: " << n2 << endl;
  cout << "n3: " << n3 << endl;
  cout << "n4: " << n4 << endl;

  remove_graph(n1);
  cout << endl;

  cout << "n1: " << n1 << endl;
  cout << "n2: " << n2 << endl;
  cout << "n3: " << n3 << endl;
  cout << "n4: " << n4 << endl;
}
int main()
{
  n10();
  cout << "program end" << endl;
  return 0;
}