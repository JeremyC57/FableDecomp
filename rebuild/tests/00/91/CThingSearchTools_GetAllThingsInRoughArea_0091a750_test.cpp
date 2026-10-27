// behaviour test for 0x0091A750: each thing is visited once per stamp and only matching ones are collected
#include <cstdio>
struct E { unsigned long stamp; int type; bool ok; };
int main()
{
    E e[4] = { { 0, 1, true }, { 0, 0, true }, { 5, 1, true }, { 0, 1, false } };
    unsigned long stamp = 5; int found = 0;
    for (int pass = 0; pass < 2; ++pass)
        for (int i = 0; i < 4; ++i)
            if (e[i].stamp < stamp) { e[i].stamp = stamp; if (e[i].type == 1 && e[i].ok) ++found; }
    if (found != 1) { std::printf("0091a750_TEST FAIL\n"); return 1; }
    std::printf("0091a750_TEST PASS\n");
    return 0;
}