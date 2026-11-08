// CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing* thing) const -- targeting weight of a thing: 0 when dead, 0.1 when its TC 9 component flags it (bit 2 of +0xc), the enemy preference factor for enemies of the hero, else 1
// Leaf: ?GetThingCompareMultiplier@CThingCompare_BestForTargeting2@@QBEMPAVCThing@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F9E70, 129 bytes (`ret 4`, float in st0).
// Callees: CInterfaceMap::LowerBound @ 0x004365B0, AreThingsEnemies(CThing*, CThing*) @ 0x0076D2F0 (`__fastcall`).
// Float pool: 0x0122DEDC = 0.0, 0x0122EE58 = 0.1, 0x0122DED8 = 1.0.
// Layout: CThingCompare_BestForTargeting2 (Ego_r.pdb 52 bytes): Hero +0x00, VectorCompare +0x04,
//   EnemyPreferenceFactor +0x30; retail CThing: interface bitset +0x20 (TC 9 = word +0x20 bit 9), CVectorMap +0x44,
//   Dead +0x91 bit 0; the TC 9 component's flag word at +0xc (bit 2 tested; component class not identified).
// Shape: IS_THING_ALIVE inline; the TC 9 lookup is the out-parameter GetInterface(&tc, 9) (the component re-uses eax;
//   a GetTC return value takes ecx); the hero read once.
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CTCInterface9 {                          // the component behind ETCInterfaceType 9
    unsigned char _pad[0xc];
    unsigned long m_flags;                      // +0x0c (bit 2: low-priority target)
};

bool __fastcall AreThingsEnemies(CThing* a, CThing* b);     // 0x0076D2F0
FABLE_FORCEINLINE bool IS_THING_ALIVE(CThing* p) { return p && !p->IsDead(); }

class CVectorCompare_BestForTargeting { public: unsigned long m_words[11]; };
class CThingCompare_BestForTargeting2 { public:
    CThing* Hero;                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;  // +0x04
    float EnemyPreferenceFactor;                // +0x30
    float GetThingCompareMultiplier(CThing* thing) const;
};

float CThingCompare_BestForTargeting2::GetThingCompareMultiplier(CThing* thing) const
{
    if (!IS_THING_ALIVE(thing))
        return 0.0f;
    const CTCInterface9* tc;
    if (thing->PeekTC(TCI_TARGETED, &tc) && (tc->m_flags & 4))
        return 0.1f;
    if (AreThingsEnemies(Hero, thing))
        return EnemyPreferenceFactor;
    return 1.0f;
}