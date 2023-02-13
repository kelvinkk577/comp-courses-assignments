#include "quadraticProbingHashTable.h"

QuadraticProbingHashTable::QuadraticProbingHashTable(int m, int (*hash)(string, int)) : OpenAddressingHashTable(m, hash){};

// int findNextQuadratic(int curr)
// {
//   if (curr == 0)
//     return 1;
//   int a = 1, b = a * a;
//   while (b != curr)
//   {
//     a++;
//     b = a * a;
//   }
//   a++;
//   b = a * a;
//   return b;
// }

int QuadraticProbingHashTable::hi(string k, int i) const
{
  return (hash(k, m) + i * i) % m;
}
