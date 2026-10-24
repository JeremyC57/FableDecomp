// Behaviour test for 0x0073DA50 CVectorMap<unsigned long,CRegisteredModeInfo,CKeyPairCompareLess>::Count(const unsigned long&) const
#include <cstdio>

struct CRegisteredModeInfo { int tag; };
template <class A, class B> struct pair { A first; B second; };

template <class K, class V> class CVectorMap {
public:
    typedef pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* First;
    value_type* Last;
    value_type* EndOfStorage;
    // stand-ins for the landed LowerBound 0x0073D210 / UpperBound 0x0073D250 (unsigned key, len/half walk)
    const_iterator LowerBound(const K& key) const
    {
        const value_type* first = First;
        int count = (int)(Last - First);
        while (count > 0) {
            int half = count / 2;
            const value_type* middle = first + half;
            if (middle->first < key) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
    const_iterator UpperBound(const K& key) const
    {
        const value_type* first = First;
        int count = (int)(Last - First);
        while (count > 0) {
            int half = count / 2;
            const value_type* middle = first + half;
            if (!(key < middle->first)) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
    long Count(const K& key) const
    {
        return UpperBound(key) - LowerBound(key);
    }
};

int main()
{
    pair<unsigned int, CRegisteredModeInfo> e[7];
    unsigned int keys[7] = { 1u, 3u, 3u, 3u, 9u, 0x80000000u, 0x80000000u };   // sorted unsigned
    for (int i = 0; i < 7; ++i) { e[i].first = keys[i]; e[i].second.tag = i; }
    CVectorMap<unsigned int, CRegisteredModeInfo> m;
    m.First = e; m.Last = e + 7; m.EndOfStorage = e + 7;
    int ok = 1;
    if (m.Count(3u) != 3) ok = 0;
    if (m.Count(1u) != 1) ok = 0;
    if (m.Count(0x80000000u) != 2) ok = 0;
    if (m.Count(2u) != 0 || m.Count(0u) != 0 || m.Count(0xFFFFFFFFu) != 0) ok = 0;
    m.Last = e;                                             // empty map
    if (m.Count(3u) != 0) ok = 0;
    if (!ok) { std::printf("0073da50_TEST FAIL\n"); return 1; }
    std::printf("0073da50_TEST PASS\n");
    return 0;
}