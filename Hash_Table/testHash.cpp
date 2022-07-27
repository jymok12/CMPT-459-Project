#include "HashTable.h"
#include <list>

int main()
{
  int key[] = {231, 321, 212, 321, 433, 262};
  int data[] = {123, 432, 523, 43, 423, 111};
  int size = sizeof(key) / sizeof(key[0]);

  HashTable h(size);

  for (int i = 0; i < size; i++)
    h.insertItem(key[i], data[i]);

  h.deleteItem(12);
  h.displayHash();
}