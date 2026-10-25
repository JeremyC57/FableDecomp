// behaviour test for 0x00AF6120 CVectorMap<EPSCType,CComponentInstance,...>::UpperBound, linked against the built
// member: first entry whose type is above the key, over runs of equal keys, before/after every entry and an empty map
#include <cstdio>
#include <cstring>
namespace NParticleEngine {
enum EPSCType { PSC_TYPE_FIRST = 0 };
class CComponentInstance {
public:
    virtual ~CComponentInstance();
    unsigned char _body[0x14 - 4];
};
}
namespace std { template <class A, class B> struct pair { A first; B second; }; }
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;
    value_type* Last;
    value_type* EndOfStorage;
    value_type* UpperBound(const K& key);
};
typedef NParticleEngine::EPSCType T;
typedef CVectorMap<T, NParticleEngine::CComponentInstance, CKeyPairCompareLess<T, NParticleEngine::CComponentInstance> > TMap;
static unsigned char g_mem[6 * 0x18];
int main()
{
    if (sizeof(TMap::value_type) != 0x18) { std::printf("00af6120_TEST FAIL size\n"); return 1; }
    std::memset(g_mem, 0, sizeof(g_mem));
    TMap::value_type* v = (TMap::value_type*)g_mem;
    const long keys[6] = { 1, 3, 3, 3, 7, 9 };
    for (int i = 0; i < 6; ++i) v[i].first = (T)keys[i];
    TMap m; m.First = v; m.Last = v + 6; m.EndOfStorage = v + 6;
    struct { long key; int want; } cases[] = { { 0, 0 }, { 1, 1 }, { 2, 1 }, { 3, 4 }, { 5, 4 }, { 7, 5 }, { 9, 6 }, { 10, 6 } };
    bool ok = true;
    for (int c = 0; c < 8; ++c) {
        T k = (T)cases[c].key;
        if (m.UpperBound(k) != v + cases[c].want) ok = false;
    }
    TMap e; e.First = e.Last = e.EndOfStorage = v;
    T k = (T)3;
    if (e.UpperBound(k) != v) ok = false;
    if (!ok) { std::printf("00af6120_TEST FAIL\n"); return 1; }
    std::printf("00af6120_TEST PASS\n");
    return 0;
}
