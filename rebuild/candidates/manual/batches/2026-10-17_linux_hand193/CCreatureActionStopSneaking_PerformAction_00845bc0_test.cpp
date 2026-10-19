#include <cstdio>
struct Mode { int removed; Mode():removed(0){} void RemoveMode(unsigned long n){ if(n==7) removed=1; } };
struct Sneak { unsigned char pad[15]; unsigned char Active; Sneak():Active(1){} };
int main(){Mode m; Sneak s; m.RemoveMode(7); s.Active=0; if(!m.removed||s.Active){std::printf("00845bc0_TEST FAIL\n");return 1;}std::printf("00845bc0_TEST PASS\n");return 0;}
