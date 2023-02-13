#include "doubleHashingHashTable.h"

DoubleHashingHashTable::DoubleHashingHashTable(int m, int (*hash)(string, int), int (*hash2)(string, int)) : OpenAddressingHashTable(m, hash)
{
  this->hash2 = hash2;
}

int DoubleHashingHashTable::hi(string k, int i) const
{
  return (hash(k, m) + i * hash2(k, m)) % m;
}