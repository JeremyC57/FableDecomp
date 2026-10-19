#include <cstdio>
struct CHitLocationDef { unsigned char b[0x7c]; };
int main(){ CHitLocationDef x; if ((char*)&x.b[0x78]-(char*)x.b != 0x78) { std::printf("004585fe_TEST FAIL\n"); return 1; } std::printf("004585fe_TEST PASS\n"); return 0; }
