#include "../cxxopts.h"
#include "HashTable.h"
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <random>

typedef std::mt19937 rng_type;
std::uniform_int_distribution<rng_type::result_type> udist(0, CAPACITY * 5);

#define DEFAULT_INPUT_SIZE "10000000"
#define DEFAULT_GROUP_SIZE "1"

void testHashTable(uint input_size, uint group_size)
{
  HashTable *ht = create_table(CAPACITY);

  rng_type rng;

  rng_type::result_type const seedval = time(NULL);
  rng.seed(seedval);

  for (uint i = 0; i < input_size; i++)
  {
    // rng_type::result_type random_number = udist(rng);
    // ht_insert(ht, random_number, i);
    // printf("%d\n", i);
    ht_insert(ht, udist(rng), i);
  }

  int val;
  int key;
  for (uint i = 0; i < group_size; i++)
  {
    key = udist(rng);
    if ((val = ht_search(ht, key)) == -1)
    {
      printf("%d does not exist\n", key);
    }
    else
    {
      printf("Key:%d, Value:%d\n", key, val);
    }
  }

  printf("GP\n");

  int *GP_input = new int[group_size];
  for (uint i = 0; i < group_size; i++)
  {
    GP_input[i] = udist(rng);
  }
  int *GP_results = HASH_PROBE_GP(GP_input, group_size, ht);
  for (uint i = 0; i < group_size; i++)
  {
    if (GP_results[i] == -1)
    {
      printf("%d does not exist\n", GP_input[i]);
    }
    else
    {
      printf("Key:%d, Value:%d\n", GP_input[i], GP_results[i]);
    }
  }
  delete[] GP_input;
  delete[] GP_results;

  // print_table(ht);
  free_table(ht);
}

int main(int argc, char *argv[])
{
  // Initialize command line arguments
  cxxopts::Options options("Hash Table Reading Benchmark",
                           "Time Hash Table Probing based on varying input and group sizes");
  options.add_options(
      "custom",
      {{"iSize", "Input Size",
        cxxopts::value<uint>()->default_value(DEFAULT_INPUT_SIZE)},
       {"gSize", "Group Size",
        cxxopts::value<uint>()->default_value(DEFAULT_GROUP_SIZE)}});
  auto cl_options = options.parse(argc, argv);
  uint input_size = cl_options["iSize"].as<uint>();
  uint group_size = cl_options["gSize"].as<uint>();
  // std::cout << "Input Size : " << input_size << "\n";
  // std::cout << "Group Size : " << group_size << "\n";

  testHashTable(input_size, group_size);

  return 0;
}