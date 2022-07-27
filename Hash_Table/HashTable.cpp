#include "HashTable.h"
#include <iostream>
#include <list>
#include <iostream>
#include <functional>
#include <string>

using namespace std;

struct node{
    int key;
    int value;
    node* next = nullptr;
};

struct state {
  node* node;
};

HashTable::HashTable(int c)
{
  int size = getPrime(c);
  this->capacity = size;
  table = new list<int>[capacity];
}

int HashTable::checkPrime(int n)
{
  if (n == 1 || n == 0 || n % 2 == 0)
  {
    return 0;
  }
  for (int i = 3; i < n / 2; i += 2)
  {
    if (n % i == 0)
    {
      return 0;
    }
  }
  return 1;
}

int HashTable::getPrime(int n)
{
  if (n % 2 == 0)
  {
    n++;
  }
  while (!checkPrime(n))
  {
    n += 2;
  }
  return n;
}

int HashTable::hashFunction(int key)
{
  return (key % capacity);
}

void HashTable::insertItem(int key, int data)
{
  int index = hashFunction(key);
  table[index].push_back(data);
}

void HashTable::deleteItem(int key)
{
  int index = hashFunction(key);

  list<int>::iterator i;
  for (i = table[index].begin();
       i != table[index].end(); i++)
  {
    if (*i == key)
      break;
  }

  if (i != table[index].end())
    table[index].erase(i);
}

void HashTable::displayHash()
{
  for (int i = 0; i < capacity; i++)
  {
    cout << "table[" << i << "]";
    for (auto x : table[i])
      cout << " --> " << x;
    cout << endl;
  }
}

void __builtin_prefetch (const void *addr);
int* HASH_PROBE_GP(const int input[], int n, HashTable hash){
    int* value = new int[n];
    state* stateArr = new state[n];
    for (int i = 0; i < n; i++) {
        // stateArr[i].node = hash.hashFunction(input[i]);
        // __builtin_prefetch(stateArr[i].node);
    }
    int num_finished = 0;
    while (num_finished < n){
        for (int i = 0; i < n; i++){
            if (stateArr[i].node == NULL){
                continue;
            }
            else if (i == stateArr[i].node->key){
                value[i] = stateArr[i].node->value;
                stateArr[i].node = nullptr;
                num_finished++;
            }
            else{
                // state[idx].node = hash.get(input[idx])
                //__builtin_prefetch(state[i].node);
            }
        };
    };

    return value;

};