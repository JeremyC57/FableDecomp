// Naming wave 18: the interface-presence stand-in CTCBitsPair is std::bitset<274> (STLport 4.6); its Test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members begin/end are the header CArray base's begin() / end() (STLport vector start +0 / finish +4); the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0).
#pragma optimize("s",on)

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
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
        CTCMapPair::value_type* entry = current_map->LowerBound(key);
        CTCMapPair::value_type* end = current_map->end();
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
        CTCMapPair::value_type* entry = current_map->LowerBound(local_key);
        CTCMapPair::value_type* end = current_map->end();
        if (entry == end || entry->first > key) entry = end;
        *output = (int)entry->second;
        result = true;
    } else {
        result = false;
    }
    return result;
}