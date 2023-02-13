#include "shop.h"
#include <iostream>
#include <cctype> //hint: you may use the isupper function in it
using std::cout;
using std::endl;

Shop::Shop(HashTable *fireTable, HashTable *iceTable, HashTable *lightningTable)
{
  profit = 0;
  this->fireTable = fireTable;
  this->iceTable = iceTable;
  this->lightningTable = lightningTable;
}

Shop::~Shop()
{
  delete fireTable;
  delete iceTable;
  delete lightningTable;
}

HashTable *Shop::getTable(string prefix) const
{
  for (int i = 0; i < prefix.length(); i++)
    prefix[i] = tolower(prefix[i]);

  if (prefix == "fire")
    return fireTable;
  if (prefix == "ice")
    return iceTable;
  if (prefix == "lightning")
    return lightningTable;

  return nullptr;
}

bool Shop::stockUp(string name, int quantity, int price) const
{
  string prefix, suffix;
  HashTable *table = nullptr;

  for (int i = 1; i < name.length(); i++)
  {
    if (isupper(name[i]))
    {
      prefix = name.substr(0, i);
      suffix = name.substr(i);
      break;
    }
  }

  table = getTable(prefix);
  if (!table)
    return false;

  Magic *magic = table->get(suffix);

  if (!magic)
  {
    magic = new Magic{prefix, suffix, price, quantity};
    if (!table->add(magic))
      return false;

    return true;
  }
  magic->quantity += quantity;
  magic->price = price;
  return true;
}

bool Shop::sell(string name, int quantity)
{
  string prefix, suffix;
  HashTable *table = nullptr;

  for (int i = 1; i < name.length(); i++)
  {
    if (isupper(name[i]))
    {
      prefix = name.substr(0, i);
      suffix = name.substr(i);
      break;
    }
  }

  table = getTable(prefix);
  if (!table)
    return false;

  Magic *magic = table->get(suffix);
  if (!magic)
    return false;

  if (magic->quantity >= quantity)
  {
    magic->quantity -= quantity;
    profit += quantity * magic->price;
    if (magic->quantity <= 0)
      table->remove(suffix);
    return true;
  }

  return false;
}
