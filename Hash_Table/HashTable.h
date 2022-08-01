#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 100000 // Size of the Hash Table

unsigned long hash_function(int str);

typedef struct Data Data;
typedef struct Node Node;
typedef struct HashTable HashTable;

// Define the Hash Table Item here
struct Data
{
  int key;
  int value;
};

// Define the Linkedlist here
struct Node
{
  Data *data;
  Node *next;
};

// Define the Hash Table here
struct HashTable
{
  // Contains an array of pointers
  // to items
  Node **nodes;
  int size;
  int count;
};

static Node *allocate_node();
static Node *node_insert(Node *list, Data *item);

static void free_linkedlist(Node *list);

static Node **create_overflow_buckets(HashTable *table);
static void free_buckets(HashTable *table);

Data *create_item(int key, int value);
HashTable *create_table(int size);

void free_table(HashTable *table);
void handle_collision(HashTable *table, unsigned long index, Data *item);
void ht_insert(HashTable *table, int key, int value);

int ht_search(HashTable *table, int key);

struct state {
  Data* node;
};

int* HASH_PROBE_GP(int input[], int n, HashTable* table);

Data *ht_get(HashTable *table, int key);

void ht_delete(HashTable *table, int key);
void print_table(HashTable *table);