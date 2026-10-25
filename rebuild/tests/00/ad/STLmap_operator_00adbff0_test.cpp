// Behaviour test for 0x00ADBFF0 map<unsigned long, CGroupInfo>::operator[] (insert default {0, -1} when absent).
#include <cstdio>
#include <map>

struct CGroupInfo { unsigned long Count; long BlockIndex; CGroupInfo() : Count(0), BlockIndex(-1) { } };
typedef std::map<unsigned long, CGroupInfo> SGroupMap;
static int g_inserts = 0;
// reconstructed body: lower_bound, insert value_type(k, CGroupInfo()) with the hint when the key is absent
static CGroupInfo& Index(SGroupMap& m, const unsigned long& k)
{
    SGroupMap::iterator i = m.lower_bound(k);
    if (i == m.end() || m.key_comp()(k, (*i).first)) { ++g_inserts; i = m.insert(i, SGroupMap::value_type(k, CGroupInfo())); }
    return (*i).second;
}

int main()
{
    SGroupMap m;
    CGroupInfo& a = Index(m, 7);
    if (a.Count != 0 || a.BlockIndex != -1 || m.size() != 1 || g_inserts != 1) { std::printf("00adbff0_TEST FAIL\n"); return 1; }
    a.Count = 3; a.BlockIndex = 2;
    Index(m, 4).BlockIndex = 9; Index(m, 0xFFFFFFF0ul).Count = 5;   // unsigned key above 0x7FFFFFFF
    if (Index(m, 7).Count != 3 || Index(m, 7).BlockIndex != 2 || g_inserts != 3) { std::printf("00adbff0_TEST FAIL\n"); return 1; }
    if (m.begin()->first != 4 || m.rbegin()->first != 0xFFFFFFF0ul || Index(m, 0xFFFFFFF0ul).BlockIndex != -1 || m.size() != 3) { std::printf("00adbff0_TEST FAIL\n"); return 1; }
    std::printf("00adbff0_TEST PASS\n");
    return 0;
}