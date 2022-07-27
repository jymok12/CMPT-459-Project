#pragma once
#include <iostream>
#include <list>

using namespace std;

typedef struct HT_item HT_item;
typedef struct LinkedList LinkedList;
typedef struct HashTable HashTable;

class HashTable
{
  int capacity;
  list<int> *table;

public:
  HashTable(int V);
  void insertItem(int key, int data);
  void deleteItem(int key);

  int checkPrime(int n);
  int getPrime(int n);

  int hashFunction(int key);

  void displayHash();
};