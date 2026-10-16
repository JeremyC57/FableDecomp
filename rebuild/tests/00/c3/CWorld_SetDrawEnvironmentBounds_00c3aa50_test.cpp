#include <cstdio>
#include <cstring>
struct R { unsigned int count; unsigned int first, n, cursor; unsigned int invalid;
  void Clear() { first = n = cursor = 0; }
  int Set(unsigned int f, unsigned int c) { if (f > count) { Clear(); return -1; } if (f + c > count) { Clear(); return -1; } if (f == invalid) { Clear(); return -1; } n = c; first = f; cursor = 0; return 0; } };
static bool Run()
{
    R r; r.count = 10; r.invalid = 7; r.first = 1; r.n = 1; r.cursor = 5;
    if (r.Set(2, 3) != 0 || r.first != 2 || r.n != 3 || r.cursor != 0) return false;
    if (r.Set(11, 0) != -1 || r.first != 0 || r.n != 0) return false;
    if (r.Set(5, 6) != -1) return false;
    return r.Set(7, 1) == -1 && r.Set(9, 1) == 0;
}
int main()
{
    if (!Run()) { std::printf("00c3aa50_TEST FAIL\n"); return 1; }
    std::printf("00c3aa50_TEST PASS\n");
    return 0;
}