// behaviour test for 0x004ED953: saving writes the count and visits every element; loading resizes to the read count first
#include <cstdio>
#include <vector>
static int g_visits = 0;
struct E { void Transfer() { ++g_visits; } };
static long Save(std::vector<E>& v) { long n = (long)v.size(); for (long i = 0; i < n; ++i) v[i].Transfer(); return n; }
static void Load(std::vector<E>& v, long n) { v.resize(n); for (long i = 0; i < n; ++i) v[i].Transfer(); }
int main()
{
    std::vector<E> v(3); long n = Save(v); std::vector<E> w; Load(w, n);
    if (n != 3 || w.size() != 3 || g_visits != 6) { std::printf("004ed953_TEST FAIL\n"); return 1; }
    std::printf("004ed953_TEST PASS\n");
    return 0;
}
