#include <iostream>
#include "polynomial.h"

using namespace std;

void c()
{
  Polynomial p;

  int arr[1] = {1};
  Polynomial p2(arr, 1);
  Polynomial result = p.add(p2);
  
  cout << "p: "; 
  p.print();
  cout << endl;

  cout << "p2: "; 
  p2.print();
  cout << endl;
  
  cout << "Result: "; 
  result.print();
  cout << endl;
}

void c2()
{
  int arr[4] = {1,2,3,4};
  Polynomial p(arr, 4);
  Polynomial p2 = p;
  Polynomial result = p2.subtract(p);
  result.print();
}

void c3()
{
  int arr[4] = {1,2,3,4};
  Polynomial p(arr, 4);
  Polynomial p2;
  Polynomial result = p.subtract(p2);
  result.print();
}

void c4()
{
  int arr[1] = {0};
  Polynomial p(arr, 1);
  Polynomial p2;
  Polynomial result = p2.multiply(p2);
  result.print();
}

void c5()
{
  int arr[4] = {0,0,0,0};
  Polynomial p(arr, 4);
  Polynomial p2 = p;
  p2.print();
  Polynomial result = p.multiply(p2);
  result.print();
}

void c6()
{
  int arr[4] = {0,1,0,0};
  Polynomial p(arr, 4);
  Polynomial p2 = p;
  Polynomial result = p.multiply(p2);
  result.print();
}

void c7()
{
  int arr[2] = {5,0};
  Polynomial p(arr, 2);
  int arr2[4] = {1,3,2,4};
  Polynomial p2(arr2, 4);
  Polynomial result = p.multiply(p2);
  result.print();
  cout << endl;
  cout << result.evaluate(2.6) << endl;
}

void c8()
{
  int arr[4] = {5,-0,0,654681235};
  Polynomial p(arr, 4);
  p.print();
}


int main()
{
  c8();
}