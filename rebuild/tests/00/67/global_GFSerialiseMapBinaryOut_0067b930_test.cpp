#include <cstdio>
#include <map>
#include <vector>
// Behaviour model: the count, then (key, value) pairs in ascending key order.
static std::vector<long> g_out;
int main()
{
    std::map<long, long> m;
    m[7] = 70; m[2] = 20; m[5] = 50;
    g_out.push_back((long)m.size());
    for (std::map<long, long>::const_iterator it = m.begin(); it != m.end(); ++it) { g_out.push_back(it->first); g_out.push_back(it->second); }
    long want[] = { 3, 2, 20, 5, 50, 7, 70 };
    for (int i = 0; i < 7; ++i) if (g_out[i] != want[i]) { std::printf("0067b930_TEST FAIL\n"); return 1; }
    std::printf("0067b930_TEST PASS\n");
    return 0;
}