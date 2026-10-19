#include <cstdio>
struct Model { unsigned long target, filter, slots[7]; };
int main(){ Model m; m.target=0; m.filter=0; for(int i=0;i<7;i++)m.slots[i]=0; if(m.target||m.filter) { std::printf("00743be0_TEST FAIL\n"); return 1;} std::printf("00743be0_TEST PASS\n"); return 0; }
