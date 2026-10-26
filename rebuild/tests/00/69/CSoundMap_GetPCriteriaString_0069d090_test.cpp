// Behaviour test for 0x0069D090 CSoundMap::GetPCriteriaString: the name's CRC (an empty CCharString hashes the
// shared "") is looked up in the sorted criteria map; a hit returns the entry's def string, a miss (between keys,
// past the end, or an empty map) returns null.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif

class CCRC {
public:
    static unsigned int __fastcall Calc(unsigned int seed, const void* data, unsigned int length)
    {
        // stand-in hash (the real CRC-32 is not under test): FNV-1a seeded with `seed`
        unsigned int h = 2166136261u ^ seed;
        const unsigned char* p = (const unsigned char*)data;
        for (unsigned int i = 0; i < length; ++i) { h ^= p[i]; h *= 16777619u; }
        return h;
    }
};
class CCharStringData { public: char* PData; unsigned int StringLength; };
static const char g_EmptyString[] = "";
class CCharString {
public:
    CCharStringData* PStringData;
    const char* GetPString() const { return PStringData == 0 ? g_EmptyString : PStringData->PData; }
    unsigned int GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    unsigned int GetCRC() const { return CCRC::Calc(0, GetPString(), GetLength()); }
};
class CDefString { public: int Index; };
template <class A, class B> struct pair { A first; B second; };

template <class K, class V> class CVectorMap {
public:
    typedef pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* First;
    value_type* Last;
    const_iterator end() const { return Last; }
    const_iterator LowerBound(const K& key) const
    {
        int count = (int)(Last - First);
        const value_type* first = First;
        while (count > 0) {
            int half = count / 2;
            const value_type* middle = first + half;
            if (middle->first < key) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
};

class CSoundMap {
public:
    CVectorMap<unsigned int, CDefString> CriteriaMap;
    const CDefString* GetPCriteriaString(const CCharString& name) const
    {
        unsigned int crc = name.GetCRC();
        CVectorMap<unsigned int, CDefString>::const_iterator it = CriteriaMap.Find(crc);
        if (it != CriteriaMap.end())
            return &it->second;
        return 0;
    }
};

static CCharString Make(CCharStringData& data, char* text)
{
    data.PData = text;
    data.StringLength = (unsigned int)std::strlen(text);
    CCharString s;
    s.PStringData = &data;
    return s;
}

int main()
{
    char fire[] = "FIRE_CRACKLE", wind[] = "WIND", none[] = "NOPE", empty[] = "";
    CCharStringData d1, d2, d3, d4;
    CCharString sFire = Make(d1, fire), sWind = Make(d2, wind), sNone = Make(d3, none), sEmpty = Make(d4, empty);
    CCharString sNull;
    sNull.PStringData = 0;

    pair<unsigned int, CDefString> entries[3];
    unsigned int k[3] = { sFire.GetCRC(), sWind.GetCRC(), sNull.GetCRC() };
    // sort the three keys
    for (int i = 0; i < 3; ++i)
        for (int j = i + 1; j < 3; ++j)
            if (k[j] < k[i]) { unsigned int t = k[i]; k[i] = k[j]; k[j] = t; }
    for (int i = 0; i < 3; ++i) { entries[i].first = k[i]; entries[i].second.Index = 100 + i; }

    CSoundMap map;
    map.CriteriaMap.First = entries;
    map.CriteriaMap.Last = entries + 3;
    const CDefString* p = map.GetPCriteriaString(sFire);
    if (!p || p->Index < 100 || p->Index > 102) {
        std::printf("0069d090_TEST FAIL\n"); return 1;
    }
    for (int i = 0; i < 3; ++i) {
        const CCharString& s = (entries[i].first == sFire.GetCRC()) ? sFire : (entries[i].first == sWind.GetCRC()) ? sWind : sNull;
        if (map.GetPCriteriaString(s) != &entries[i].second) { std::printf("0069d090_TEST FAIL\n"); return 1; }
    }
    // an empty CCharString with data hashes like the null one (both hash "")
    if (map.GetPCriteriaString(sEmpty) != map.GetPCriteriaString(sNull) || !map.GetPCriteriaString(sEmpty)) {
        std::printf("0069d090_TEST FAIL\n"); return 1;
    }
    if (map.GetPCriteriaString(sNone) != 0) { std::printf("0069d090_TEST FAIL\n"); return 1; }
    map.CriteriaMap.Last = entries;                         // empty map
    if (map.GetPCriteriaString(sFire) != 0) { std::printf("0069d090_TEST FAIL\n"); return 1; }
    std::printf("0069d090_TEST PASS\n");
    return 0;
}