#include <iostream>
#include <functional>
#include <string>
#include "HashTable.h"

using namespace std;


struct AMAC_state
{
  int key;
  Node *node;
  int value;
  int stage;
};

struct AMAC_circular_buffer
{
    int group_size;
    int next = 0;
    AMAC_state *stateArr;

    AMAC_circular_buffer(int n) {
      group_size = n;
      stateArr = new AMAC_state[n];
      for(int i; i < group_size; i++) {
        stateArr[i].stage = 0;
      }
    }

    AMAC_state next_state() {
        int curr_next = next;
        next = (next + 1) % group_size;
        return stateArr[curr_next];
    }
};


int *HASH_PROBE_AMAC(int *input, int n, HashTable *table, int group_size)
{
  int *value = new int[n];
  AMAC_circular_buffer buff = AMAC_circular_buffer(group_size);
  int num_finished, i, j = 0;
  while (num_finished < n) {
    AMAC_state state = buff.next_state();
    if (state.stage == 0){
      state.key = input[i++];
      state.node = table->nodes[hash_function(state.key)];
      state.stage = 1;
      __builtin_prefetch(state.node);
    } else if (state.stage == 1) {
      if (state.node == nullptr) {
        state.value = -1;
        state.stage = 0;
        value[j++] = state.value;
        num_finished++;
        continue;
      }
      else if (state.key == state.node->data->key) {
        state.value = state.node->data->value;
        state.stage = 0;
        value[j++] = state.value;
        num_finished++;
      } else {
        state.node = state.node->next;
        __builtin_prefetch(state.node);
      }
    }
  }

  return value;
};