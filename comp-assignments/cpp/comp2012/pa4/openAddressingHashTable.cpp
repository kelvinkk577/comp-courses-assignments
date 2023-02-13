#include "openAddressingHashTable.h"

OpenAddressingHashTable::OpenAddressingHashTable(int m, int (*hash)(string, int)) : HashTable(m, hash)
{
  table = new Cell[m];
  for (int i = 0; i < m; i++)
  {
    table[i].magic = nullptr;
    table[i].status = CellStatus::EMPTY;
  }
}

OpenAddressingHashTable::~OpenAddressingHashTable()
{
  for (int i = 0; i < m; i++)
  {
    delete table[i].magic;
  }
  delete[] table;
}

bool OpenAddressingHashTable::add(Magic *magic)
{
  int i = hash(magic->suffix, m);
  if (printSteps)
    cout << "hash(\"" << magic->suffix << "\") = " << i << endl;

  if (activeCellCount > (m / 2.0))
  {
    if (printSteps)
      cout << "Rehashing is needed!" << endl;
    performRehashing();
    i = hash(magic->suffix, m);
  }

  int colCount = 0;

  while (table[i].status == CellStatus::ACTIVE)
  {
    if (colCount >= m)
    {
      comparisonCount += m;
      if (printSteps)
        cout << magic->prefix << magic->suffix << " cannot be added" << endl;
      return false;
    }
    if (printSteps)
      cout << magic->prefix << magic->suffix << " collided at cell " << i << endl;
    colCount++;
    i = hi(magic->suffix, colCount);
  }
  table[i].magic = magic;
  table[i].status = CellStatus::ACTIVE;
  activeCellCount++;
  comparisonCount += (colCount + 1);
  if (printSteps)
    cout << magic->prefix << magic->suffix << " added at cell " << i << endl;
  return true;
}

void OpenAddressingHashTable::performRehashing()
{
  m = 2 * m;
  Cell *newTable = new Cell[m];
  for (int k = 0; k < m; k++)
  {
    newTable[k].magic = nullptr;
    newTable[k].status = CellStatus::EMPTY;
  }

  for (int j = 0; j < (m / 2); j++)
  {
    if (table[j].status != CellStatus::ACTIVE)
      continue;
    Magic *magic = table[j].magic;
    int colCount = 0;
    int i = hash(magic->suffix, m);
    if (printSteps)
      cout << "hash(\"" << magic->suffix << "\") = " << i << endl;
    while (newTable[i].status == CellStatus::ACTIVE)
    {
      if (printSteps)
        cout << magic->prefix << magic->suffix << " collided at cell " << i << endl;
      colCount++;
      i = hi(magic->suffix, colCount);
    }

    newTable[i].magic = magic;
    newTable[i].status = CellStatus::ACTIVE;
    // activeCellCount++;
    comparisonCount += (colCount + 1);
    if (printSteps)
      cout << magic->prefix << magic->suffix << " added at cell " << i << endl;
  }

  delete[] table;

  table = newTable;

  if (printSteps)
    cout << "Rehashing is done!" << endl;
}

bool OpenAddressingHashTable::remove(string key)
{
  int i = hash(key, m);

  if (printSteps)
    cout << "hash(\"" << key << "\") = " << i << endl;

  int C = 0;
  bool found = false;
  while (!found)
  {
    if (C >= m)
    {
      comparisonCount += m;
      if (printSteps)
        cout << key << " cannot be removed" << endl;
      return false;
    }

    if (table[i].status == CellStatus::EMPTY)
    {
      C++;
      comparisonCount += C;
      if (printSteps)
      {
        cout << "visited cell " << i << " but could not find it" << endl;
        cout << key << " cannot be removed" << endl;
      }
      return false;
    }

    if (table[i].status == CellStatus::ACTIVE && table[i].magic->suffix == key) // found
    {
      found = true;
      if (printSteps)
        cout << table[i].magic->prefix + table[i].magic->suffix << " removed at cell " << i << endl;

      activeCellCount--;
      delete table[i].magic;
      table[i].magic = nullptr;
      table[i].status = CellStatus::DELETED;
      comparisonCount += C + 1;
      return true;
    }

    if ((table[i].status == CellStatus::DELETED) || (table[i].status == CellStatus::ACTIVE && table[i].magic->suffix != key)) // Deleted or Collided
    {
      if (printSteps)
        cout << "visited cell " << i << " but could not find it" << endl;
      C++;
      i = hi(key, C);
    }
  }
  return found;
}

Magic *OpenAddressingHashTable::get(string key)
{
  int i = hash(key, m);

  if (printSteps)
    cout << "hash(\"" << key << "\") = " << i << endl;

  int C = 0;
  bool found = false;
  while (!found)
  {
    if (C >= m)
    {
      comparisonCount += m;
      if (printSteps)
        cout << key << " cannot be found" << endl;
      return nullptr;
    }

    if (table[i].status == CellStatus::EMPTY)
    {
      C++;
      comparisonCount += C;
      if (printSteps)
      {
        cout << "visited cell " << i << " but could not find it" << endl;
        cout << key << " cannot be found" << endl;
      }
      return nullptr;
    }

    if (table[i].status == CellStatus::ACTIVE && table[i].magic->suffix == key) // found
    {
      found = true;
      if (printSteps)
        cout << table[i].magic->prefix + table[i].magic->suffix << " found at cell " << i << endl;

      comparisonCount += C + 1;
      return table[i].magic;
    }

    if ((table[i].status == CellStatus::DELETED) || (table[i].status == CellStatus::ACTIVE && table[i].magic->suffix != key)) // Deleted or Collided
    {
      if (printSteps)
        cout << "visited cell " << i << " but could not find it" << endl;
      C++;
      i = hi(key, C);
    }
  }
  return nullptr;
}

int OpenAddressingHashTable::getClusterCount() const
{
  if (m == 0)
    return 0;
  if (m == 1)
  {
    if (table[0].status == CellStatus::ACTIVE)
      return 1;
    else
      return 0;
  }
  int clus = 0;
  for (int i = 1; i < m; i++)
  {
    bool prev = table[i - 1].status == CellStatus::ACTIVE;
    bool curr = table[i].status == CellStatus::ACTIVE;
    if (curr == false && prev == true) // curr cell is empty/deleted and prev is active (i.e. end of cluster)
      clus++;
  }
  if (table[m - 1].status == CellStatus::ACTIVE) // increment cluster count if last cell is active
  {
    if (table[0].status != CellStatus::ACTIVE)
      clus++;
  }
  return clus;
}

int OpenAddressingHashTable::getLargestClusterSize() const
{
  if (getClusterCount() == 0)
    return 0;
  if (m == 1)
    if (table[0].status == CellStatus::ACTIVE)
      return 1;

  int size, max;
  size = max = 0;
  for (int i = 0; i < m; i++)
  {
    if (table[i].status == CellStatus::ACTIVE)
      size++;
    else
    {
      if (size > max)
        max = size;
      size = 0;
    }
  }
  if (size > max)
    max = size;

  if (table[0].status == CellStatus::ACTIVE && table[m - 1].status == CellStatus::ACTIVE)
  {
    int headNTail = 0;
    for (int i = 0; i < m; i++)
      if (table[i].status == CellStatus::ACTIVE)
        headNTail++;
      else
        break;
    for (int i = m - 1; i >= 0; i--)
      if (table[i].status == CellStatus::ACTIVE)
        headNTail++;
      else
        break;

    if (headNTail > max)
      max = headNTail;
  }
  return max;
}

string OpenAddressingHashTable::getClusterSizeSortedList() const
{
  int clusCount = getClusterCount();

  if (clusCount == 0)
    return "(empty)";

  int *sizes = new int[clusCount]{0};

  bool circular = (table[0].status == CellStatus::ACTIVE && table[m - 1].status == CellStatus::ACTIVE ? true : false);

  int index = 0;
  for (int i = 0; i < clusCount;)
  {
    int size = 0;
    for (; index < m; index++)
    {
      if (table[index].status == CellStatus::ACTIVE)
      {
        size++;
      }
      else
      {
        if (index - 1 < 0)
          continue;
        if (table[index - 1].status == CellStatus::ACTIVE)
        {
          sizes[i] = size;
          size = 0;
          i++;
        }
      }
      if (index == m - 1)
      {
        if (i < clusCount)
        {
          sizes[i] = size;
          i++;
        }
      }
    }
  }

  if (circular)
  {
    int headNTail = 0;
    for (int i = 0; i < m; i++)
      if (table[i].status == CellStatus::ACTIVE)
        headNTail++;
      else
        break;
    for (int i = m - 1; i >= 0; i--)
      if (table[i].status == CellStatus::ACTIVE)
        headNTail++;
      else
        break;

    sizes[0] = headNTail;
  }

  for (int i = 0; i < clusCount - 1; i++)
  {
    for (int j = 0; j < clusCount - i - 1; j++)
    {
      if (sizes[j] < sizes[j + 1])
      {
        int temp = sizes[j];
        sizes[j] = sizes[j + 1];
        sizes[j + 1] = temp;
      }
    }
  }

  string result = "";
  for (int i = 0; i < clusCount; i++)
  {
    result += std::to_string(sizes[i]);
    if (i != clusCount - 1)
      result += ",";
  }
  delete[] sizes;
  return result;
}