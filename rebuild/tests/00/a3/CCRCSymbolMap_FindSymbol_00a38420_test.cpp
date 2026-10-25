#include <cstdio>
#include <string.h>

static const char g_empty[] = "";
struct CCharStringData {
    const char*   PData;
    unsigned long StringLength;
};
class CCharString {
public:
    CCharStringData* PStringData;
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetPString() const { return PStringData == 0 ? g_empty : PStringData->PData; }
};

// test stand-in for Calc 0x004014A0 (CRC-32): any deterministic hash of (seed, chars, length) will do
static unsigned long Calc(unsigned long seed, const void* data, unsigned long length)
{
    unsigned long h = seed ^ 2166136261u;
    const unsigned char* p = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i)
        h = ((h ^ p[i]) * 16777619u) & 0xffffffffu;
    return h;
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp* end() const { return _M_finish; }
};
}

template <class K, class V> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;
    bool                     Dirty;
    const value_type* LowerBound(const K& key) const
    {
        const value_type* first = Data._M_start;
        long len = (long)(Data._M_finish - first);
        while (len > 0) {
            long half = len / 2;
            if (first[half].first < key) { first += half + 1; len -= half + 1; }
            else len = half;
        }
        return first;
    }
    const value_type* End() const { return Data.end(); }
    const value_type* Find(const K& key) const
    {
        const value_type* it = LowerBound(key);
        if (it == Data.end() || key < it->first)
            return Data.end();
        return it;
    }
};

class CCRCSymbolMap {
public:
    CVectorMap<unsigned long, long> SLONGMap;
    bool FindSymbol(const CCharString& name, long& value) const;
};

bool CCRCSymbolMap::FindSymbol(const CCharString& name, long& value) const
{
    const CVectorMap<unsigned long, long>::value_type* it = SLONGMap.Find(Calc(0, name.GetPString(), name.GetLength()));
    if (it != SLONGMap.End()) {
        value = it->second;
        return true;
    }
    return false;
}

static void MakeString(CCharString& s, CCharStringData& d, const char* text)
{
    d.PData = text;
    d.StringLength = (unsigned long)strlen(text);
    s.PStringData = &d;
}

int main()
{
    const char* names[4] = { "ENUM_A", "ENUM_B", "ENUM_C", "" };
    std::pair<unsigned long, long> entries[4];
    for (int i = 0; i < 4; ++i) {
        entries[i].first = Calc(0, names[i], (unsigned long)strlen(names[i]));
        entries[i].second = 10 * (i + 1);
    }
    for (int i = 0; i < 4; ++i)                           // sort by key (CVectorMap order)
        for (int j = i + 1; j < 4; ++j)
            if (entries[j].first < entries[i].first) {
                std::pair<unsigned long, long> t = entries[i];
                entries[i] = entries[j];
                entries[j] = t;
            }
    CCRCSymbolMap map;
    map.SLONGMap.Data._M_start = entries;
    map.SLONGMap.Data._M_finish = entries + 3;            // "" left out of the map
    map.SLONGMap.Data._M_end_of_storage = entries + 4;
    for (int i = 0; i < 3; ++i)
        if (entries[i].first == Calc(0, "", 0)) {         // keep the three named symbols in the live range
            entries[i] = entries[3];
        }
    for (int i = 0; i < 3; ++i)
        for (int j = i + 1; j < 3; ++j)
            if (entries[j].first < entries[i].first) {
                std::pair<unsigned long, long> t = entries[i];
                entries[i] = entries[j];
                entries[j] = t;
            }

    CCharString s;
    CCharStringData d;
    long value = -1;
    MakeString(s, d, "ENUM_B");
    if (!map.FindSymbol(s, value) || value != 20)
        { std::printf("00a38420_TEST FAIL\n"); return 1; }
    MakeString(s, d, "ENUM_A");
    if (!map.FindSymbol(s, value) || value != 10)
        { std::printf("00a38420_TEST FAIL\n"); return 1; }
    value = -1;
    MakeString(s, d, "ENUM_Z");
    if (map.FindSymbol(s, value) || value != -1)
        { std::printf("00a38420_TEST FAIL\n"); return 1; }
    CCharString empty;
    empty.PStringData = 0;                                 // null block: hashes the "" literal, not in the map
    if (map.FindSymbol(empty, value) || value != -1)
        { std::printf("00a38420_TEST FAIL\n"); return 1; }
    std::printf("00a38420_TEST PASS\n");
    return 0;
}