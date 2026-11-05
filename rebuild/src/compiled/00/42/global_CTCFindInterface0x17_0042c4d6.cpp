// Naming wave 18: the interface-presence stand-in CTCBitsRemaining is std::bitset<274> (STLport 4.6); its Test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members begin/end are the header CArray base's begin() / end() (STLport vector start +0 / finish +4); the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0).
#pragma optimize("s",on)

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMapRemaining;

#define CTC_LAYOUT(METHOD) \
    unsigned char unused00[0x20]; std::bitset<274> bits; CTCMapRemaining map; \
    bool METHOD(int* output)

#define DEFINE_SMALL(CLASS, METHOD, KEY) \
struct CLASS { CTC_LAYOUT(METHOD); }; \
bool CLASS::METHOD(int* output) { \
    unsigned long tested = bits.test(KEY); bool result; \
    if ((unsigned char)tested != 0) { \
        ETCInterfaceType key = (ETCInterfaceType)(KEY); CTCMapRemaining* current_map = &(map); \
        CTCMapRemaining::value_type* entry = current_map->LowerBound(key); CTCMapRemaining::value_type* end = current_map->end(); \
        if (entry == end || entry->first > KEY) entry = end; \
        *output = (int)entry->second; result = true; \
    } else result = false; \
    return result; \
}

#define DEFINE_LARGE(CLASS, METHOD, KEY) \
struct CLASS { CTC_LAYOUT(METHOD); }; \
bool CLASS::METHOD(int* output) { \
    register int key = KEY; unsigned long tested = bits.test(key); bool result; \
    if ((unsigned char)tested != 0) { \
        CTCMapRemaining* current_map = &(map); ETCInterfaceType local_key = (ETCInterfaceType)(key); \
        CTCMapRemaining::value_type* entry = current_map->LowerBound(local_key); CTCMapRemaining::value_type* end = current_map->end(); \
        if (entry == end || entry->first > key) entry = end; \
        *output = (int)entry->second; result = true; \
    } else result = false; \
    return result; \
}

DEFINE_LARGE(CTC10e, CTC_FindInterface_0x10e, 0x10e)
DEFINE_SMALL(CTC17, CTC_FindInterface_0x17, 0x17)
DEFINE_SMALL(CTC19, CTCBase_FindInterfaceType_0x19, 0x19)
DEFINE_SMALL(CTC16, CTCBase_FindInterfaceType_0x16, 0x16)
DEFINE_SMALL(CTC29, CTCBase_FindInterfaceType_0x29, 0x29)
DEFINE_SMALL(CTC1b, CTCBase_FindInterfaceType_0x1b, 0x1b)
DEFINE_SMALL(CTC6f, CTCBase_FindInterfaceType_0x6f, 0x6f)
DEFINE_SMALL(CTC13, CTCBase_FindInterfaceType_0x13, 0x13)
DEFINE_SMALL(CTC14, CTCBase_FindInterfaceType_0x14, 0x14)
DEFINE_SMALL(CTC12, CTCBase_FindInterfaceType_0x12, 0x12)
DEFINE_LARGE(CTCf6, CTCBase_FindInterfaceType_0xf6, 0xf6)
DEFINE_LARGE(CTC10d, CTCBase_FindInterfaceType_0x10d, 0x10d)
DEFINE_LARGE(CTCd0, CTCBase_FindInterfaceType_0xd0, 0xd0)
DEFINE_LARGE(CTCfc, CTCBase_FindInterfaceType_0xfc, 0xfc)