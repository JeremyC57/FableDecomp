#include <cstdio>
struct CThing {}; struct F { CThing* t; bool operator()(CThing* x) { return (x != t) ? true : false; } };
int main()
{
    CThing a, b; F f; f.t = &a;
    if (f(&a) || !f(&b)) { std::printf("00661640_TEST FAIL\n"); return 1; }

    std::printf("00661640_TEST PASS\n");
    return 0;
}
