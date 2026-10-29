#pragma optimize("s",on)

struct CTCBitsRemaining { unsigned long words[9]; unsigned long Test(unsigned long bit); };
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public: typedef std::pair<ETCInterfaceType, CTCBase*> SEntry; SEntry* begin; SEntry* end; SEntry* LowerBound(const ETCInterfaceType& key); };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMapRemaining;

#define CTC_LAYOUT(METHOD) \
    unsigned char unused00[0x20]; CTCBitsRemaining bits; CTCMapRemaining map; \
    bool METHOD(int* output)

#define DEFINE_SMALL(CLASS, METHOD, KEY) \
struct CLASS { CTC_LAYOUT(METHOD); }; \
bool CLASS::METHOD(int* output) { \
    unsigned long tested = bits.Test(KEY); bool result; \
    if ((unsigned char)tested != 0) { \
        ETCInterfaceType key = (ETCInterfaceType)(KEY); CTCMapRemaining* current_map = &(map); \
        CTCMapRemaining::SEntry* entry = current_map->LowerBound(key); CTCMapRemaining::SEntry* end = current_map->end; \
        if (entry == end || entry->first > KEY) entry = end; \
        *output = (int)entry->second; result = true; \
    } else result = false; \
    return result; \
}

#define DEFINE_LARGE(CLASS, METHOD, KEY) \
struct CLASS { CTC_LAYOUT(METHOD); }; \
bool CLASS::METHOD(int* output) { \
    register int key = KEY; unsigned long tested = bits.Test(key); bool result; \
    if ((unsigned char)tested != 0) { \
        CTCMapRemaining* current_map = &(map); ETCInterfaceType local_key = (ETCInterfaceType)(key); \
        CTCMapRemaining::SEntry* entry = current_map->LowerBound(local_key); CTCMapRemaining::SEntry* end = current_map->end; \
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