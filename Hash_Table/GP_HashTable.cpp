// #include <iostream>
// #include <functional>
// #include <string>

// using namespace std;

// struct node{
//     int key;
//     int value;
//     node* next = nullptr;
// };

// void __builtin_prefetch (const void *addr);
// int* HASH_PROBE_GP(node* input[], int n, hash<string> hash){
//     int* value = new int[n];
//     node* state = new node[n];
//     for (int i = 0; i < n; i++) {
//         state[i] = hash<int>(input[i]);
//         __builtin_prefetch(state[i].node);
//     }
//     int num_finished = 0;
//     while (num_finished < sizeof(input)){
//         for (int i = 0; i < n; i++){
//             if (i == NULL){
//                 continue;
//             }
//             else if (i == state[i]){
//                 value = state;
//                 state = NULL;
//                 num_finished++;
//             }
//             else{
//                 // state[idx].node = hash.get(input[idx])
//                 //__builtin_prefetch(state[i].node);
//             }
//         };
//     };

//     return value;

// };