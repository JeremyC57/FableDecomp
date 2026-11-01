// Naming wave 18: the interface-presence stand-in CTCBitsPair is std::bitset<274> (STLport 4.6); its Test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
#pragma optimize("s",on)

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public: typedef std::pair<ETCInterfaceType, CTCBase*> SEntry; SEntry* begin; SEntry* end; SEntry* LowerBound(const ETCInterfaceType& key); };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMapPair;

struct CTCInterface4
{
    unsigned char unused00[0x20];
    std::bitset<274> bits;
    CTCMapPair map;
    bool CTC_FindInterface_4(int* output);
};

bool CTCInterface4::CTC_FindInterface_4(int* output)
{
    unsigned long tested = bits.test(4);
    bool result;
    if ((unsigned char)tested != 0) {
        ETCInterfaceType key = (ETCInterfaceType)(4);
        CTCMapPair* current_map = &(map);
        CTCMapPair::SEntry* entry = current_map->LowerBound(key);
        CTCMapPair::SEntry* end = current_map->end;
        if (entry == end || entry->first > 4) entry = end;
        *output = (int)entry->second;
        result = true;
    } else {
        result = false;
    }
    return result;
}

struct CTCInterfaceE8
{
    unsigned char unused00[0x20];
    std::bitset<274> bits;
    CTCMapPair map;
    bool CTCBase_FindInterfaceType_0xe8(int* output);
};

bool CTCInterfaceE8::CTCBase_FindInterfaceType_0xe8(int* output)
{
    register int key = 0xe8;
    unsigned long tested = bits.test(key);
    bool result;
    if ((unsigned char)tested != 0) {
        CTCMapPair* current_map = &(map);
        ETCInterfaceType local_key = (ETCInterfaceType)(key);
        CTCMapPair::SEntry* entry = current_map->LowerBound(local_key);
        CTCMapPair::SEntry* end = current_map->end;
        if (entry == end || entry->first > key) entry = end;
        *output = (int)entry->second;
        result = true;
    } else {
        result = false;
    }
    return result;
}