#include "../cxxopts.h"
#include "HashTable.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <chrono>

#define DEFAULT_INPUT_SIZE "10000000"
#define DEFAULT_GROUP_SIZE "25"

typedef std::mt19937 rng_type;
std::uniform_int_distribution<rng_type::result_type> udist(0, CAPACITY * 5);

void print_results(int *input, int *results, uint group_size)
{
  for (uint i = 0; i < group_size; i++)
  {
    if (results[i] == -1)
    {
      printf("%d does not exist\n", input[i]);
    }
    else
    {
      printf("Key:%d, Value:%d\n", input[i], results[i]);
    }
  }
}

HashTable *createUniformTable(uint input_size, rng_type rng)
{
  HashTable *ht = create_table(CAPACITY);
  for (uint i = 0; i < input_size; i++)
  {
    ht_insert(ht, udist(rng), i);
  }
  return ht;
}

void testHashTable(uint input_size, uint group_size)
{
  rng_type rng;

  rng_type::result_type const seedval = time(NULL);
  rng.seed(seedval);

  HashTable *ht = createUniformTable(input_size, rng);

  int *input = new int[group_size];
  int *GP_input = new int[group_size];
  int *AMAC_input = new int[group_size];
  int *CORO_input = new int[group_size];
  for (uint i = 0; i < group_size; i++)
  {
    input[i] = udist(rng);
    GP_input[i] = udist(rng);
    AMAC_input[i] = udist(rng);
    CORO_input[i] = udist(rng);
  }

  // printf("NAIVE\n");

  auto start = std::chrono::steady_clock::now();
  int *results = HASH_PROBE(input, group_size, ht);
  auto end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
  // print_results(input, results, group_size);

  delete[] input;
  delete[] results;
  free_table(ht);

  // printf("GP\n");
  ht = createUniformTable(input_size, rng);

  start = std::chrono::steady_clock::now();
  int *GP_results = HASH_PROBE_GP(GP_input, group_size, ht);
  end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
  // print_results(GP_input, GP_results, group_size);

  delete[] GP_input;
  delete[] GP_results;
  free_table(ht);

  // printf("AMAC\n");
  ht = createUniformTable(input_size, rng);

  start = std::chrono::steady_clock::now();
  int *AMAC_results = HASH_PROBE_AMAC(AMAC_input, group_size, ht, group_size);
  end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
  // print_results(AMAC_input, AMAC_results, group_size);

  delete[] AMAC_input;
  delete[] AMAC_results;
  free_table(ht);

  // printf("CORO\n");
  ht = createUniformTable(input_size, rng);

  int *CORO_results = (int *)malloc(sizeof(int) * group_size);
  std::vector<ReturnObject> coroutine_promises(group_size);
  
  start = std::chrono::steady_clock::now();

  for (int i = 0; i < group_size; i++)
  {
    coroutine_promises[i] = HASH_PROBE_CORO(ht, CORO_input[i]);
  }
  while (std::any_of(coroutine_promises.begin(), coroutine_promises.end(), [](ReturnObject x)
                     { return !x.h_.done(); }))
  {
    for (int i = 0; i < group_size; i++)
    {
      if (!coroutine_promises[i].h_.done())
      {
        coroutine_promises[i].h_.resume();
      }
    }
  }
  end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << std::endl;
  for (int i = 0; i < group_size; i++)
  {
    CORO_results[i] = coroutine_promises[i].h_.promise().val_;
  }
  // print_results(CORO_input, CORO_results, group_size);

  free_table(ht);
}

int main(int argc, char *argv[])
{
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

  testHashTable(input_size, group_size);

  return 0;
}