#include "linearProbingHashTable.h"
#include "quadraticProbingHashTable.h"
#include "shop.h"

using namespace std;

int (*stringLength)(string, int) = [](string s, int m)
{
  // cout << "yooyoo";
  int hash = (int)s.length() % m;
  // cout << "the hash is " << hash << endl;
  return hash;
};

void printStats(HashTable *hashTable, bool activeCellCount, bool comparisonCount, bool clusterCount, bool largestClusterSize, bool clusterSizeList)
{
  if (activeCellCount)
    cout << "Active cell count = " << hashTable->getActiveCellCount() << endl;
  if (comparisonCount)
    cout << "Accumulated comparison count = " << hashTable->getAccumulatedComparisonCount() << endl;
  if (clusterCount)
    cout << "Cluster count = " << hashTable->getClusterCount() << endl;
  if (largestClusterSize)
    cout << "Largest cluster size = " << hashTable->getLargestClusterSize() << endl;
  if (clusterSizeList)
    cout << "Cluster size list = " << hashTable->getClusterSizeSortedList() << endl;
}

int main()
{
  HashTable *fire = new LinearProbingHashTable(3, stringLength);
  HashTable *ice = new LinearProbingHashTable(3, stringLength);
  HashTable *lightning = new LinearProbingHashTable(3, stringLength);
  Shop shop(fire, ice, lightning);
  shop.stockUp("FireBall", 20, 100);
  shop.stockUp("IceRain", 30, 20);
  shop.stockUp("LightningStorm", 10, 50);
  shop.print();

  cout << endl;
  shop.sell("FireBall", 21);
  shop.print();

  cout << endl;
  shop.sell("FireBall", 10);
  shop.print();

  cout << endl;
  shop.sell("FireBall", 10);
  shop.print();

  cout << endl;
  if (!shop.sell("IceVain", 21))
    cout << "IceVain is not found!" << endl;

  shop.print();
  // HashTable *hashTable = new LinearProbingHashTable(3, stringLength);
  // hashTable->togglePrintSteps();
  // hashTable->add(new Magic{"Fire", "Ball", 100, 10});
  // hashTable->add(new Magic{"Fire", "Win", 200, 20});
  // hashTable->add(new Magic{"Fire", "Storms", 300, 30});
  // hashTable->add(new Magic{"Fire", "Ox", 400, 40});

  // // HashTable *hashTable = new QuadraticProbingHashTable(3, stringLength);

  // // hashTable->add(new Magic{"Fire", "Ball", 400, 10});
  // // hashTable->add(new Magic{"Fire", "Duck", 400, 10});
  // // hashTable->add(new Magic{"Fire", "Bull", 400, 10});
  // // hashTable->add(new Magic{"Fire", "Doge", 400, 10});
  // hashTable->print();
  // cout << endl;
  // printStats(hashTable, true, true, true, true, true);
}