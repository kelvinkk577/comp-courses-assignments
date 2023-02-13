#include "polynomial.h"
#include <iostream>
using namespace std;
//note: you are not allowed to include any additional library, see also FAQ for what you can/cannot do
//write your code below and submit this file only

Polynomial::Polynomial()
{
  head = nullptr;
}

Polynomial::Polynomial(const Polynomial& another)
{
  if (another.head == nullptr)
  {
    head = nullptr;
    return;
  }
  head = new Term;
  head->coefficient = another.head->coefficient;
  head->exponent = another.head->exponent;
  
  Term* prev = head;

  for (Term* t=another.head->next; t; t = t->next)
  {
    Term* clone = new Term;
    clone->coefficient = t->coefficient;
    clone->exponent = t->exponent;
    prev->next = clone;

    prev = clone;  
  }

  prev->next = nullptr;
}

Polynomial::Polynomial(int array[], int arraySize)
{
  int index = 0;
  for (; index < arraySize; index++)
  {
    if (array[index] != 0)
      break;
  }

  if (index > arraySize-1)
  {
    head = nullptr;
    return;
  }

  head = new Term;
  head->coefficient = array[index];
  head->exponent = arraySize-index-1;

  index += 1;

  Term* prev = head;
  for (; index < arraySize; index++)
  {
    if (array[index] == 0)
      continue;
    Term* node = new Term;
    node->coefficient = array[index];
    node->exponent = arraySize-index-1;
    prev->next = node;

    prev = node;
  }

  prev->next = nullptr;
}

Polynomial::~Polynomial()
{
  for (Term* t=head; t;)
  {
    Term* next = t->next;
    delete t;
    t = next;
  }
  head = nullptr;
}

void Polynomial::print() const
{
  if (!head)
  {
    cout << "0";
    return;
  }

  for (Term* t=head; t; t=t->next)
  {
    if (t->coefficient > 0 && t != head)
      cout << "+";
    else if (t->coefficient < 0)
      cout << "-";
    
    if (t != head)
      cout << " ";
    
    if (t->exponent == 0)
    {
      if (t->coefficient < 0)
        cout << -t->coefficient;
      else
        cout << t->coefficient;
      return;
    }
    
    if (t->coefficient != 1 && t->coefficient != -1)
    {
      if (t->coefficient < 0)
        cout << -t->coefficient;
      else
        cout << t->coefficient;
      cout << "*";
    }

    cout << "x";
    
    if (t->exponent != 1)
      cout << "^" << t->exponent;
    
    if (t->next)
      cout << " ";
  }
}

Polynomial Polynomial::add(const Polynomial& another) const
{
  if (head == nullptr)
    return Polynomial(another);
  if (another.head == nullptr)
    return Polynomial(*this);
  
  int maxExponent = head->exponent;
  if (another.head->exponent > maxExponent)
    maxExponent = another.head->exponent;
  
  int arraySize = maxExponent + 1;
  int array[arraySize] = {};
  Term *t, *t2;
  t = head;
  t2 = another.head;

  for (int i = 0; i < arraySize; i++)
  {
    int currExpo = maxExponent - i;
    int sum = 0;

    if (t && (t->exponent == currExpo))
    {
      sum += t->coefficient;
      t = t->next;
    }

    if (t2 && (t2->exponent == currExpo))
    {
      sum += t2->coefficient;
      t2 = t2->next;
    }

    array[i] = sum;
  }

  return Polynomial(array, arraySize);
}

Polynomial Polynomial::subtract(const Polynomial& another) const
{
  if (another.head == nullptr)
    return Polynomial(*this);

  int arraySize = another.head->exponent + 1;
  int array[arraySize] = {};
  Term* t = another.head;
  for (int i = 0; i < arraySize; i++)
  {
    int currExpo = another.head->exponent - i;
    if (t && (t->exponent == currExpo))
    {
      array[i] = -t->coefficient;
      t = t->next;
    }
  }

  return add(Polynomial(array, arraySize));
}

Polynomial Polynomial::multiply(const Polynomial& another) const
{
  if (head == nullptr || another.head == nullptr)
    return Polynomial();

  int len1, len2;
  len1 = len2 = 0;
  for (Term* t = head; t; t = t->next)
    len1++;
  for (Term* t = another.head; t; t = t->next)
    len2++;
  
  int maxArraySize = len1 * len2;

  int coeArr[maxArraySize] = {};
  int expArr[maxArraySize] = {};

  int i = 0;
  for (Term* t = head; t; t = t->next)
  {
    for (Term* t2 = another.head; t2; t2 = t2->next)
    {
      coeArr[i] = t->coefficient * t2->coefficient;
      expArr[i] = t->exponent + t2->exponent;
      i++;
    }
  }

  int maxExponent = expArr[0];
  for (int j = 0; j < maxArraySize; j++)
    if (expArr[j] > maxExponent)
      maxExponent = expArr[j];
  
  int resultArraySize = maxExponent + 1;
  int resultArray[resultArraySize] = {};

  for (int j = 0; j < resultArraySize; j++)
  {
    int currExpo = maxExponent - j;
    for (int k = 0; k < maxArraySize; k++)
    {
      if (expArr[k] == currExpo)
        resultArray[j] += coeArr[k];
    }
  }

  return Polynomial(resultArray, resultArraySize);
}

int Polynomial::evaluate(int valueOfX) const
{
  int result = 0;
  for (Term* t = head; t; t = t->next)
  {
    if (t->coefficient == 0)
      continue;

    if (t->exponent == 0)
      result += t->coefficient;
    else
    {
      int temp = 1;
      for (int i = 0; i < t->exponent; i++)
        temp *= valueOfX;
      result += t->coefficient * temp;
    }
  }

  return result;
}

int Polynomial::compare(const Polynomial& another) const
{
  if (head == nullptr && another.head == nullptr)
    return 0;
  if (head == nullptr)
    return -1;
  if (another.head == nullptr)
    return 1;
  
  int len1, len2;
  len1 = len2 = 0;
  for (Term* t = head; t; t = t->next)
    len1++;
  for (Term* t = another.head; t; t = t->next)
    len2++;
  
  int& shortLen = len1;
  if (len2 < shortLen)
    shortLen = len2;
  
  Term* t = head;
  Term* t2 = another.head;

  for (int i = 0; i < shortLen; i++)
  {
    if (t->exponent > t2->exponent)
      return 1;
    else if (t->exponent < t2->exponent)
      return -1;
    else
    {
      if (t->coefficient > t2->coefficient)
        return 1;
      else if (t->coefficient < t2->coefficient)
        return -1;
      else
      {
        t = t->next;
        t2 = t2->next;
      }
    }
  }

  if (t && !t2)
    return 1;
  if (!t && t2)
    return -1; 
    
  return 0;   
}

// void print_terms(Term* head)
// {
//   for (Term* t = head; t; t=t->next)
//   {
//     cout << "Addr of Node: " << &*t << endl;
//     cout << "Coefficient: " << t->coefficient << endl;
//     cout << "Exponent: " << t->exponent << endl;
//     cout << "Addr of Next: " << &*t->next << endl;
//     cout << endl;
//   }
//   cout << "==========================" << endl << endl;
// }

// int main()
// {
  // Polynomial p(2);
  // // print_terms(p.head);
  // p.print();

  // Polynomial p2(p);
  // // print_terms(p2.head);
  // p2.print();

  // int arr[3] = {-1,-6, -7};
  // Polynomial p3(arr, 3);
  // // print_terms(p3.head);
  // p3.print();

  // int arr2[5] = {0,0,11,-5,0};
  // Polynomial p4(arr2, 5);
  // print_terms(p4.head);
  // p4.print();

  // int arr3[5] = {2};
  // Polynomial p5(arr3, 1);
  // // print_terms(p5.head);
  // p5.print();

  // int arr4[1] = {0};
  // Polynomial p6(arr4, 1);
  // // print_terms(p6.head);
  // p6.print();
    
// }


