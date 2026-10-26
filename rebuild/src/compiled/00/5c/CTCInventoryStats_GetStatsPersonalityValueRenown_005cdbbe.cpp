// CTCInventoryStats::GetStatsPersonalityValueRenown() -- the hero's renown as a fraction: the owner thing's CTCHeroStats RenownLevel over RenownMaxLevel, 0 when the thing has no hero stats or the maximum is 0
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- a same-TU read-only noinline LowerBound body (0x0040F020, the escaping callee) frees the key word for the fidiv divisor spill
// Leaf: ?GetStatsPersonalityValueRenown@CTCInventoryStats@@QAEMXZ
// Retail: Fable.exe @ 0x005CDBBE, 94 bytes, __fastcall `ret`, float in st0. Ego_r.pdb
// `?GetStatsPersonalityValueRenown@CTCInventoryStats@@QAEMXZ`. Caller: CTCVillage::OnCreate 0x00659221 (on the player
// character's interface 0x14, scaled by the village def's renown factor).
// Callees: std::bitset::test @ 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the
// inlined CThing::GetInterface for interface 4 = CTCHeroStats; the find result is not null-checked before ->second).
// Layout: retail CTCHeroStats RenownLevel +0x70, RenownMaxLevel +0x74 (engine/CTCHeroStats.h is the PDB layout, +0x78/+0x7C;
// the landed CTCHeroStats rows use +0x70); CThing interface bits +0x20, interface map +0x44 (landed GetInterface model).
// Shape: frame-pointer size TU; the result local is assigned on two paths, so its 0.0f is stored up front (`fldz ; fstp`)
// and reloaded on every miss while the hit keeps `fild level ; fidiv max` in st0 (the max spilled into the dead
// lookup-key slot as a compiler temporary); returning the quotient directly folds the 0.0f into a constant load (92v94),
// and a named `maxLevel` local takes a third frame word (95v94).
// Label: the catalog's ETCInterface_CalculateRatio is this method.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCHeroStats : public CTCHeroStatsDonor {
                                    // interface type 4 (retail offsets)
public:
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType,CTCBase*>
    struct Entry { long type; void* component; };
    Entry* first; Entry* last;
    Entry* LowerBound(const long*);                     // 0x0040F020
};
struct CPresenceBits { bool test(long kind); };         // bitset<>::test 0x00410DE0
// LowerBound @ 0x0040F020: a same-TU read-only stand-in (GOTCHAS 665, the escaping callee) -- with its body visible the
// key word is dead after the call and VC7.1 reuses it for the fidiv divisor spill as retail.
#pragma auto_inline(off)
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
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x20]; CPresenceBits present; unsigned char _pad21[0x23]; CInterfaceMap interfaces;
    template<class T> __forceinline bool GetInterface(T** out, long kind) {
        if (present.test(kind)) {
            long key = kind; CInterfaceMap* map = &interfaces; CInterfaceMap::Entry* it = map->LowerBound(&key);
            if (it == map->last || kind < it->type) it = map->last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
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