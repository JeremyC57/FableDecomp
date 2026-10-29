#pragma optimize("s",on)

struct CTCBitsPair
{
    unsigned long words[9];
    unsigned long Test(unsigned long bit);
};

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
    CTCBitsPair bits;
    CTCMapPair map;
    bool CTC_FindInterface_4(int* output);
};

bool CTCInterface4::CTC_FindInterface_4(int* output)
{
    unsigned long tested = bits.Test(4);
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
    CTCBitsPair bits;
    CTCMapPair map;
    bool CTCBase_FindInterfaceType_0xe8(int* output);
};

bool CTCInterfaceE8::CTCBase_FindInterfaceType_0xe8(int* output)
{
    register int key = 0xe8;
    unsigned long tested = bits.Test(key);
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