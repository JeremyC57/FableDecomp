// Test for 0x0073D250 CVectorMap<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo, ...>::UpperBound: links the
// source object (its explicit instantiation) and checks the result against a linear scan for every prefix length
// and probe key, duplicates included.
#include <stdio.h>
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
class CThing;
class CTCCreatureModeManager {
public:
    class CModeBase;
    class CRegisteredModeInfo { public: CModeBase* (__cdecl* AllocFunc)(CThing*); };
};
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;
    value_type* Last;
    value_type* EndOfStorage;
    value_type* UpperBound(const K& key) const;
};
typedef CTCCreatureModeManager::CRegisteredModeInfo RMI;
typedef CVectorMap<unsigned long, RMI, CKeyPairCompareLess<unsigned long, RMI> > Map;
int main()
{
    Map::value_type e[8];
    unsigned long keys[8] = { 1, 2, 2, 5, 7, 7, 9, 11 };
    for (int i = 0; i < 8; ++i) { e[i].first = keys[i]; e[i].second.AllocFunc = 0; }
    bool ok = sizeof(Map::value_type) == 8;
    for (int n = 0; n <= 8 && ok; ++n) {
        Map m; m.First = e; m.Last = e + n; m.EndOfStorage = e + 8;
        for (unsigned long probe = 0; probe < 13; ++probe) {
            int expect = 0;
            while (expect < n && !(probe < e[expect].first)) ++expect;
            if (m.UpperBound(probe) != e + expect) { ok = false; break; }
        }
    }
    if (!ok) { puts("0073d250_TEST FAIL"); return 1; }
    puts("0073d250_TEST PASS");
    return 0;
}
