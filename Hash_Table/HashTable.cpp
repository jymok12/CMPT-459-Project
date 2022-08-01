#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"

unsigned long hash_function(int n)
{
  return n % CAPACITY;
}

static Node *allocate_node()
{
  Node *node = (Node *)malloc(sizeof(Node));
  return node;
}

static Node *node_insert(Node *list, Data *data)
{
  if (list == nullptr)
  {
    Node *head = allocate_node();
    head->data = data;
    head->next = nullptr;
    list = head;
    return list;
  }
  else if (list->next == nullptr)
  {
    Node *node = allocate_node();
    node->data = data;
    node->next = nullptr;
    list->next = node;
    return list;
  }

  Node *temp = list;
  while (temp->next->next != nullptr)
  {
    temp = temp->next;
  }

  Node *node = allocate_node();
  node->data = data;
  node->next = nullptr;
  temp->next = node;

  return list;
}

static void free_linkedlist(Node *list)
{
  Node *temp = list;
  while (list != nullptr)
  {
    temp = list;
    list = list->next;
    free(temp->data);
    free(temp);
  }
}

static void free_buckets(HashTable *table)
{
  Node **buckets = table->nodes;
  for (int i = 0; i < table->size; i++)
    free_linkedlist(buckets[i]);
  free(buckets);
}

Data *create_data(int key, int value)
{
  Data *data = (Data *)malloc(sizeof(Data));
  data->key = key;
  data->value = value;
  return data;
}

HashTable *create_table(int size)
{
  // Creates a new HashTable
  HashTable *table = (HashTable *)malloc(sizeof(HashTable));
  table->size = size;
  table->count = 0;
  table->nodes = (Node **)calloc(table->size, sizeof(Node *));
  for (int i = 0; i < table->size; i++)
    table->nodes[i] = nullptr;

  return table;
}

void free_table(HashTable *table)
{
  free_buckets(table);
  free(table);
}

void handle_collision(HashTable *table, unsigned long index, Data *data)
{
  Node *head = table->nodes[index];

  if (head == nullptr)
  {
    head = allocate_node();
    head->data = data;
    table->nodes[index] = head;
    return;
  }
  else
  {
    table->nodes[index] = node_insert(head, data);
    return;
  }
}

void ht_insert(HashTable *table, int key, int value)
{
  // Create the data
  Data *data = create_data(key, value);

  // Compute the index
  unsigned long index = hash_function(key);

  Node *curr = table->nodes[index];

  if (curr == nullptr)
  {
    if (table->count == table->size)
    {
      // Hash Table Full
      // printf("Insert Error: Hash Table is full\n");
      // Remove the create data
      free(data);
      // printf("HASH TABLE FULL\n");
      return;
    }

    // Insert directly
    table->nodes[index] = node_insert(table->nodes[index], data);
    // printf("%d: %d\n", table->nodes[index]->data->key, table->nodes[index]->data->value);
    table->count++;
  }
  else
  {
    if (curr->data->key == key)
    {
      table->nodes[index]->data->value = value;
      return;
    }
    else
    {
      handle_collision(table, index, data);
      return;
    }
  }
}

int ht_search(HashTable *table, int key)
{
  // Searches the key in the hashtable
  // and returns -1 if it doesn't exist
  int index = hash_function(key);
  Node *curr = table->nodes[index];

  // Ensure that we move to items which are not nullptr
  while (curr != nullptr && curr->data != nullptr)
  {
    if (curr->data->key == key)
      return curr->data->value;
    curr = curr->next;
  }
  return -1;
}

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

Data *ht_get(HashTable *table, int key)
{
  int index = hash_function(key);
  if (table->nodes[index])
  {
    return table->nodes[index]->data;
  }
  return nullptr;
}

void ht_delete(HashTable *table, int key)
{
  // Deletes an data from the table
  int index = hash_function(key);
  Node *head = table->nodes[index];

  Node *curr = head;
  Node *prev = nullptr;

  while (curr && curr->data)
  {
    if (curr->data->key == key)
    {
      if (prev == nullptr)
      {
        // First element of the chain. Remove the chain
        free_linkedlist(head);
        table->count--;
        table->nodes[index] = nullptr;
        return;
      }
      else
      {
        // This is somewhere in the chain
        prev->next = curr->next;
        curr->next = nullptr;
        free_linkedlist(curr);
        // table->nodes[index] = head;
        return;
      }
    }
    curr = curr->next;
    prev = curr;
  }
  return;
}

void print_table(HashTable *table)
{
  printf("\n-------------------\n");
  for (int i = 0; i < table->size; i++)
  {
    if (table->nodes[i] != nullptr)
    {
      printf("Index:%d, Key:%d, Value:%d", i, table->nodes[i]->data->key, table->nodes[i]->data->value);
      if (table->nodes[i]->next != nullptr)
      {
        printf(" => Overflow Bucket => ");
        Node *head = table->nodes[i]->next;
        while (head)
        {
          printf("Key:%d, Value:%d ", head->data->key, head->data->value);
          head = head->next;
        }
      }
      printf("\n");
    }
  }
  printf("-------------------\n");
}