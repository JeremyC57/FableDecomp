#include <cstdio>
struct X { unsigned char p[4]; unsigned char a; unsigned char b; unsigned char q[2]; unsigned char c; void F() { a = 0; b = 0; c = 0; } };
int main()
{
    X x; x.a = x.b = x.c = 1; x.F(); if (x.a || x.b || x.c) { std::printf("00e0f870_TEST FAIL\n"); return 1; }

    std::printf("00e0f870_TEST PASS\n");
    return 0;
}