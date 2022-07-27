#include <iostream>
#include <functional>
#include <string>
#include "HashTable.h"

using namespace std;

struct state {
  Ht_item* node;
};

char** HASH_PROBE_GP(char* input[], int n, HashTable* table){
    char** value = new char*[n];
    state* stateArr = new state[n];
    for (int i = 0; i < n; i++) {
        stateArr[i].node = ht_get(table, input[i]);
        __builtin_prefetch(stateArr[i].node);
    }
    int num_finished = 0;
    while (num_finished < n){
        for (int i = 0; i < n; i++){
            if (stateArr[i].node == NULL){
                continue;
            }
            else if (input[i] == stateArr[i].node->key){
                value[i] = stateArr[i].node->value;
                stateArr[i].node = nullptr;
                num_finished++;
            }
            else{
                stateArr[i].node = ht_get(table, input[i]);
                __builtin_prefetch(stateArr[i].node);
            }
        };
    };

    return value;

};