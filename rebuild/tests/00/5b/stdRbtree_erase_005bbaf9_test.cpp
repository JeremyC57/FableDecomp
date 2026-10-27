// behaviour test for 0x005BBAF9: erasing a node destroys its value, frees it and decrements the count
#include <cstdio>
#include <map>
static int g_dtor = 0;
struct V { int v; V(int x = 0) : v(x) { } V(const V& o) : v(o.v) { } ~V() { ++g_dtor; } };
int main()
{
    std::map<long, V> m; m.insert(std::make_pair(1L, V(10))); m.insert(std::make_pair(2L, V(20)));
    int before = g_dtor; m.erase(m.find(1L));
    if (m.size() != 1 || g_dtor != before + 1 || m.begin()->second.v != 20) { std::printf("005bbaf9_TEST FAIL\n"); return 1; }
    std::printf("005bbaf9_TEST PASS\n");
    return 0;
}