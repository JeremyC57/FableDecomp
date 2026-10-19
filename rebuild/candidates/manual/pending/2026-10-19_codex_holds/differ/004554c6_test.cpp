#include <cstdio>
struct Obj { int n; bool live; Obj():n(0),live(true){} void release(){if(live){live=false;++n;}} };
int main(){ Obj x; x.release(); if(x.live||x.n!=1){std::printf("004554c6_TEST FAIL\n");return 1;} std::printf("004554c6_TEST PASS\n"); return 0; }