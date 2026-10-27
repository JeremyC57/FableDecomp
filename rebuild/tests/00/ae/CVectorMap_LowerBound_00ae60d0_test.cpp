// Behaviour test for 0x00AE60D0 CVectorMap<CCharString,unsigned long,CKeyPairCompareLess<..> >::LowerBound(const CCharString&) const
// Model: binary search with the CCharString operator< (same data -> not less, null name sorts first, null key is never greater).
#include <cstdio>
#include <cstring>
class CCharStringData { public: const char* m_pData; };
class CCharString { public:
    CCharStringData* PStringData;
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (PStringData == 0) return true;
        if (other.PStringData == 0) return false;
        return std::strcmp(PStringData->m_pData, other.PStringData->m_pData) < 0;
    }
};
struct Entry { CCharString first; unsigned long second; };
struct TMap {
    Entry* _M_start; Entry* _M_finish;
    const Entry* LowerBound(const CCharString& key) const
    {
        int count = (int)(_M_finish - _M_start);
        const Entry* first = _M_start;
        while (count > 0) {
            int half = count / 2;
            const Entry* middle = first + half;
            if (middle->first < key) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
};
int main()
{
    CCharStringData d[3] = { { "apple" }, { "kiwi" }, { "pear" } };
    Entry e[4];
    e[0].first.PStringData = 0;    e[0].second = 0;          // the null name sorts first
    for (int i = 0; i < 3; ++i) { e[i + 1].first.PStringData = &d[i]; e[i + 1].second = i + 1; }
    TMap m; m._M_start = e; m._M_finish = e + 4;
    CCharStringData kd = { "kiwi" };
    CCharString k; k.PStringData = &kd;
    if (m.LowerBound(k) != e + 2) { std::printf("00ae60d0_TEST FAIL\n"); return 1; }
    k.PStringData = &d[2];
    if (m.LowerBound(k) != e + 3) { std::printf("00ae60d0_TEST FAIL\n"); return 1; }
    CCharStringData zd = { "zzz" }; k.PStringData = &zd;
    if (m.LowerBound(k) != e + 4) { std::printf("00ae60d0_TEST FAIL\n"); return 1; }
    k.PStringData = 0;
    if (m.LowerBound(k) != e) { std::printf("00ae60d0_TEST FAIL\n"); return 1; }
    std::printf("00ae60d0_TEST PASS\n");
    return 0;
}