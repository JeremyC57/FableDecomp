// Behaviour test for 0x00AD6420 map<unsigned long, float>::operator[] (insert 0.0f when the key is absent).
#include <cstdio>
#include <map>

typedef std::map<unsigned long, float> SWeightMap;
static int g_inserts = 0;
// reconstructed body: lower_bound, insert value_type(k, float()) with the hint when the key is absent
static float& Index(SWeightMap& m, const unsigned long& k)
{
    SWeightMap::iterator i = m.lower_bound(k);
    if (i == m.end() || m.key_comp()(k, (*i).first)) { ++g_inserts; i = m.insert(i, SWeightMap::value_type(k, float())); }
    return (*i).second;
}

static int Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    SWeightMap m;
    float& a = Index(m, 5);
    if (!Near(a, 0.0f) || m.size() != 1 || g_inserts != 1) { std::printf("00ad6420_TEST FAIL\n"); return 1; }
    a = 2.5f;
    Index(m, 0xFFFFFFF0u) = 1.25f;          // unsigned key above 0x7FFFFFFF sorts last
    Index(m, 1) += 0.5f;
    if (!Near(Index(m, 5), 2.5f) || !Near(Index(m, 1), 0.5f) || g_inserts != 3 || m.size() != 3) { std::printf("00ad6420_TEST FAIL\n"); return 1; }
    if (m.begin()->first != 1 || m.rbegin()->first != 0xFFFFFFF0u || !Near(m.rbegin()->second, 1.25f)) { std::printf("00ad6420_TEST FAIL\n"); return 1; }
    std::printf("00ad6420_TEST PASS\n");
    return 0;
}