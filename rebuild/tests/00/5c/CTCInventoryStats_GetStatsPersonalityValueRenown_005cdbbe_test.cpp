// Behaviour test for 0x005CDBBE CTCInventoryStats::GetStatsPersonalityValueRenown(): RenownLevel / RenownMaxLevel of the
// owner thing's hero stats (interface 4), 0 when it has none, the stats pointer is null or the maximum is 0.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
class CTCHeroStats {                                    // interface type 4 (retail offsets)
public:
    unsigned char _pad00[0x70];
    long RenownLevel;                                   // +0x70
    long RenownMaxLevel;                                // +0x74
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType,CTCBase*>
    struct Entry { long type; void* component; };
    Entry* first; Entry* last;
    Entry* LowerBound(const long*);                     // 0x0040F020
};
struct CPresenceBits { bool test(long kind); };         // bitset<>::test 0x00410DE0
__declspec(noinline) CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* key)
{
    Entry* lo = first;
    int len = last - first;
    while (len > 0) {
        int half = len >> 1;
        Entry* middle = lo + half;
        if (middle->type < *key) {
            lo = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return lo;
}
struct CThing {
    unsigned char _pad00[0x20]; CPresenceBits present; unsigned char _pad21[0x23]; CInterfaceMap interfaces; unsigned int bits;
    template<class T> __forceinline bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            long key = kind; CInterfaceMap* map = &interfaces; CInterfaceMap::Entry* it = map->LowerBound(&key);
            if (it == map->last || kind < it->type) it = map->last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
// test stub: the presence byte at +0x20 is a one-bit-per-kind mask for kinds 0..7
bool CPresenceBits::test(long kind) { return (*(unsigned char*)this >> kind) & 1; }

class CTCInventoryStats {
public:
    void*   __vftable;                                  // +0x00 (CTCBase)
    CThing* ParentThing;                                // +0x04
    float GetStatsPersonalityValueRenown();
};
float CTCInventoryStats::GetStatsPersonalityValueRenown()
{
    float value = 0.0f;
    CTCHeroStats* stats;
    if (ParentThing->GetInterface(&stats, 4) && stats && stats->RenownMaxLevel)
        value = (float)stats->RenownLevel / stats->RenownMaxLevel;
    return value;
}


int main()
{
    CTCHeroStats* hero = (CTCHeroStats*)malloc(sizeof(CTCHeroStats)); memset(hero, 0, sizeof(*hero));
    CInterfaceMap::Entry entries[3];
    entries[0].type = 1; entries[0].component = 0;
    entries[1].type = 4; entries[1].component = hero;
    entries[2].type = 9; entries[2].component = 0;
    CThing* thing = (CThing*)malloc(sizeof(CThing)); memset(thing, 0, sizeof(*thing));
    thing->interfaces.first = entries; thing->interfaces.last = entries + 3;
    CTCInventoryStats inv; inv.__vftable = 0; inv.ParentThing = thing;
    int ok = 1;
    hero->RenownLevel = 3; hero->RenownMaxLevel = 12;
    if (inv.GetStatsPersonalityValueRenown() != 0.0f) ok = 0;            // no interface bit
    *(unsigned char*)&thing->present = 1 << 4;
    float r = inv.GetStatsPersonalityValueRenown();
    if (r < 0.2499f || r > 0.2501f) ok = 0;
    hero->RenownMaxLevel = 0;
    if (inv.GetStatsPersonalityValueRenown() != 0.0f) ok = 0;            // a zero maximum
    entries[1].component = 0;
    if (inv.GetStatsPersonalityValueRenown() != 0.0f) ok = 0;            // a null component
    printf(ok ? "005cdbbe_TEST PASS\n" : "005cdbbe_TEST FAIL\n");
    return ok ? 0 : 1;
}