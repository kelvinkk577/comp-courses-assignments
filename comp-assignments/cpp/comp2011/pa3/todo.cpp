//modify and submit this file only
#include "pa3.h"
#include <iostream>
using namespace std;
//you are NOT allowed to include any additional library; see FAQ

int getTrainLength(TrainCar *head)
{
  int counter = 1;
  while (true)
  {
    if (head->next)
      counter++;
    else
      break;
    head = head->next;
  }
  return counter;
}

int getTrainLength(const TrainCar *head)
{
  int counter = 1;
  while (true)
  {
    if (head->next)
      counter++;
    else
      break;
    head = head->next;
  }
  return counter;
}

TrainCar *getCarOfPosition(TrainCar *head, int position)
{
  int cur = 0;

  while (true)
  {
    if (cur == position)
      return head;

    if (head->next)
    {
      cur++;
      head = head->next;
    }
    else
      break;
  }

  return nullptr;
}

const TrainCar *getCarOfPosition(const TrainCar *head, int position)
{
  int cur = 0;

  while (true)
  {
    if (cur == position)
      return head;

    if (head->next)
    {
      cur++;
      head = head->next;
    }
    else
      break;
  }

  return nullptr;
}

TrainCar *createTrainHead()
{
  TrainCar *head = new TrainCar;
  head->type = HEAD;
  head->maxLoad = 0;
  head->load = 0;
  head->prev = nullptr;
  head->next = nullptr;
  return head;
}

bool addCar(TrainCar *head, int position, CarType type, int maxLoad)
{
  if ((type == HEAD) || (position <= 0) || (maxLoad <= 0))
    return false;
  if (position > getTrainLength(head))
    return false;

  TrainCar *car = new TrainCar;
  car->type = type;
  car->maxLoad = maxLoad;
  car->load = 0;

  TrainCar *prev = getCarOfPosition(head, position - 1);
  TrainCar *next = prev->next;
  prev->next = car;
  car->prev = prev;
  car->next = next;

  if (next)
    next->prev = car;

  return true;
}

bool deleteCar(TrainCar *head, int position)
{
  if ((position < 1) || (position >= getTrainLength(head)))
    return false;

  TrainCar *car = getCarOfPosition(head, position);
  TrainCar *prev = car->prev;
  TrainCar *next = car->next;

  delete car;
  car = nullptr;

  prev->next = next;
  if (next)
    next->prev = prev;

  return true;
}

bool swapCar(TrainCar *head, int a, int b)
{
  if (a == b)
    return true;

  TrainCar *carA = getCarOfPosition(head, a);
  TrainCar *carB = getCarOfPosition(head, b);

  if (carA == nullptr || carB == nullptr)
    return false;

  CarType tempType = carA->type;
  int tempMaxLoad = carA->maxLoad;
  int tempLoad = carA->load;

  carA->type = carB->type;
  carA->maxLoad = carB->maxLoad;
  carA->load = carB->load;

  carB->type = tempType;
  carB->maxLoad = tempMaxLoad;
  carB->load = tempLoad;

  return true;
}

void sortTrain(TrainCar *head, bool ascending)
{
  int len = getTrainLength(head);
  for (int i = 0; i < len - 1; i++)
  {
    for (int j = 0; j < len - i - 1; j++)
    {
      if (getCarOfPosition(head, j)->type == HEAD)
        continue;

      if (ascending)
      {
        if (getCarOfPosition(head, j)->load > getCarOfPosition(head, j + 1)->load)
          swapCar(head, j, j + 1);
      }
      else
      {
        if (getCarOfPosition(head, j)->load < getCarOfPosition(head, j + 1)->load)
          swapCar(head, j, j + 1);
      }
    }
  }
}

bool load(TrainCar *head, CarType type, int amount)
{
  int freeSpace = 0;

  // Scan the train once to calculate free space
  for (TrainCar *car = head->next; car != nullptr; car = car->next)
    if (car->type == type)
      freeSpace += (car->maxLoad - car->load);

  // Return if not enough free space
  if (freeSpace < amount)
    return false;

  // Load the cargo
  for (TrainCar *car = head->next; car != nullptr; car = car->next)
  {
    if (car->type == type)
    {
      if (car->load + amount <= car->maxLoad)
      {
        car->load += amount;
        return true;
      }
      else
      {
        amount -= (car->maxLoad - car->load);
        car->load = car->maxLoad;
      }
    }
  }
  return true;
}

bool unload(TrainCar *head, CarType type, int amount)
{
  int cargoOnTrain = 0;
  TrainCar *car = head->next;
  bool reachedTail = false;
  while (!reachedTail)
  {
    if (car->type == type)
      cargoOnTrain += car->load;

    if (car->next)
      car = car->next;
    else
      reachedTail = true;
  }

  if (cargoOnTrain < amount)
    return false;

  for (; car->type != HEAD; car = car->prev)
  {
    if (car->type == type)
    {
      if (car->load - amount >= 0)
      {
        car->load -= amount;
        return true;
      }
      else
      {
        amount -= car->load;
        car->load = 0;
      }
    }
  }
  return true;
}

void printCargoStats(const TrainCar *head)
{
  int len = getTrainLength(head) - 1; // minus head
  CarType cargoType[len] = {HEAD};
  int loads[len], maxLoads[len];
  int pointer = 0;
  for (TrainCar *car = head->next; car != nullptr; car = car->next)
  {
    bool foundSameType = false;
    for (int i = 0; i < len; i++)
    {
      if (cargoType[i] == car->type)
      {
        foundSameType = true;
        loads[i] += car->load;
        maxLoads[i] += car->maxLoad;
        break;
      }
    }
    if (!foundSameType)
    {
      cargoType[pointer] = car->type;
      loads[pointer] = car->load;
      maxLoads[pointer] = car->maxLoad;
      pointer++;
    }
  }
  const char enumToStringMapping[6][8] = {"HEAD", "OIL", "COAL", "WOOD", "STEEL", "SUGAR"};
  for (int i = 0; i < pointer; i++)
  {
    cout << enumToStringMapping[cargoType[i]] << ":" << loads[i] << "/" << maxLoads[i];
    if (i != pointer - 1)
      cout << ",";
  }
  cout << endl;
}

void divide(const TrainCar *head, TrainCar *results[CARGO_TYPE_COUNT])
{
  for (int i = 0; i < CARGO_TYPE_COUNT; i++)
    results[i] = nullptr;
  int resultPointer = 0;
  for (TrainCar *car = head->next; car != nullptr; car = car->next)
  {
    // Create a copy
    TrainCar *copiedCar = new TrainCar;
    copiedCar->type = car->type;
    copiedCar->maxLoad = car->maxLoad;
    copiedCar->load = car->load;
    copiedCar->prev = nullptr;
    copiedCar->next = nullptr;

    bool foundSameType = false;
    for (int i = 0; i < CARGO_TYPE_COUNT; i++)
    {
      if ((results[i]) && (results[i]->next->type == car->type))
      {
        foundSameType = true;
        TrainCar *resultCar = results[i]->next;

        // Get the last car of the same type
        while (resultCar->next)
          resultCar = resultCar->next;

        resultCar->next = copiedCar; // Append the copied car to the result
        copiedCar->prev = resultCar;
        break;
      }
    }

    if (!foundSameType)
    {
      results[resultPointer] = createTrainHead();
      results[resultPointer]->next = copiedCar;
      copiedCar->prev = results[resultPointer];
      resultPointer++;
    }
  }
}

int pow(int base, int expo)
{
  int result = 1;
  for (int i = 0; i < expo; i++)
    result *= base;
  return result;
}

TrainCar *optimizeForMaximumPossibleCargos(const TrainCar *head, int upperBound)
{
  TrainCar *result_train = createTrainHead();

  if (upperBound <= 0)
    return result_train;

  TrainCar *temp_train = nullptr;

  int len = getTrainLength(head);

  int snum = 0;
  int sum = 0;
  int max = 0;
  while (snum < pow(2, len))
  {
    // if (temp_train != nullptr)
    //   deallocateTrain(temp_train);
    temp_train = createTrainHead();

    for (int i = 0; i < len; i++)
    {
      if ((snum & (1 << i)) != 0)
      {
        const TrainCar *car = getCarOfPosition(head, i);

        TrainCar *copiedCar = new TrainCar;
        copiedCar->type = car->type;
        copiedCar->maxLoad = car->maxLoad;
        copiedCar->load = car->load;
        copiedCar->prev = nullptr;
        copiedCar->next = nullptr;

        TrainCar *temp = temp_train;
        while (temp->next)
          temp = temp->next;

        temp->next = copiedCar;
        copiedCar->prev = temp;

        sum += car->load;
      }
    }
    if ((sum > max) && (sum <= upperBound))
    {
      // deallocateTrain(result_train);
      result_train = temp_train;
      max = sum;
    }
    sum = 0;
    snum++;
    // deallocateTrain(temp_train);
    // temp_train = nullptr;
  }

  return result_train;
}

void deallocateTrain(TrainCar *head)
{
  while (head != nullptr)
  {
    TrainCar *tmp = head;
    head = head->next;
    delete tmp;
    tmp = nullptr;
  }
}
