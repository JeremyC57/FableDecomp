#include <cstdio>
struct Interface { int added; void Add() { ++added; } };
int main() {
    Interface p[14] = {};
    for (int i=0;i<14;++i) p[i].Add();
    for (int i=0;i<14;++i) if (p[i].added != 1) { std::printf("006338d0_TEST FAIL\n"); return 1; }
    std::printf("006338d0_TEST PASS\n"); return 0;
}
