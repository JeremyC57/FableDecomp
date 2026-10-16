#include <cstdio>
#include <cstring>
struct Info { unsigned long uid; };
static long Find(const Info* m, unsigned long n, unsigned long uid) { for (unsigned long i = 1; i < n; ++i) if (m[i].uid == uid) return (long)i; return 0; }
static bool Run() { Info m[3] = { { 5 }, { 6 }, { 5 } }; return Find(m, 3, 5) == 2 && Find(m, 3, 6) == 1 && Find(m, 3, 9) == 0; }
int main()
{
    if (!Run()) { std::printf("004fb920_TEST FAIL\n"); return 1; }
    std::printf("004fb920_TEST PASS\n");
    return 0;
}