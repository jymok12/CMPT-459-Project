#include <iostream>
#include <functional>
#include <string>
#include "HashTable.h"

using namespace std;


struct GP_state
{
  Node *node;
};

int *HASH_PROBE_GP(int *input, int n, HashTable *table)
{
  int *value = new int[n];
  GP_state *stateArr = new GP_state[n];
  for (int i = 0; i < n; i++)
  {
    // stateArr[i].node = table->overflow_buckets[hash_function(input[i])];
    stateArr[i].node = table->nodes[hash_function(input[i])];
    __builtin_prefetch(stateArr[i].node);
  }
  int num_finished = 0;
  while (num_finished < n)
  {
    for (int i = 0; i < n; i++)
    {
      if (stateArr[i].node == nullptr)
      {
        value[i] = -1;
        num_finished++;
        continue;
      }
      else if (input[i] == stateArr[i].node->data->key)
      {
        value[i] = stateArr[i].node->data->value;
        num_finished++;
      }
      else
      {
        stateArr[i].node = stateArr[i].node->next;
        __builtin_prefetch(stateArr[i].node);
      }
    };
  };

  return value;
};