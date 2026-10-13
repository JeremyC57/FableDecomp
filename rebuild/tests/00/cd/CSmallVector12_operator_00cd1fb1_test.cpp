#include <cstdio>
struct E { long v[3]; }; struct V { E* d; E& operator[](unsigned long i) { return d[i]; } };
int main()
{
    E arr[4]; V v; v.d = arr; if (&v[3] != &arr[3]) { std::printf("00cd1fb1_TEST FAIL\n"); return 1; }

    std::printf("00cd1fb1_TEST PASS\n");
    return 0;
}