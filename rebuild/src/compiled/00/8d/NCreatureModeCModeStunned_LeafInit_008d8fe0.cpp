// NCreatureMode::CModeStunned::LeafInit() -- on entering the stun, set the stunned creature's targeting flags to 4 and take it out of combat
// Leaf: ?LeafInit@CModeStunned
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8FE0, 126 bytes (+ int3 padding), `ret`; CModeStunned vtable slot 12.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (two inlined lookups: type 9 = bit 9 of the +0x20 dword, type 0x30 = bit 16
//   of the +0x24 dword = CTCCombat), the type-9 component's flag setter @ 0x007EA270 (as the landed
//   CModeOutOfActionBase::OnDeactivate 0x008D8F30; manifest "CObservable::AddExclusiveObserver" is propagated),
//   CTCCombat::RemoveFromCombat @ 0x00733580 (landed).
// Name: FableWin.pdb `?LeafInit@CModeStunned@NCreatureMode@@EAEXXZ` (slot 12 of the out-of-action modes).
// Shape: the first lookup is the out-parameter GetInterface (the component lands as `mov eax,[eax+4] ; mov ecx,eax`), the
//   second HasTC + a returned GetTC (`mov ecx,[eax+4]`); both keys share one frame word; ParentThing is re-read.
class CTCBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCTargeted { public:                  // the type-9 component: a flags word at +0xc with an out-of-line setter
    unsigned char _pad00[0x0c];
    unsigned long m_flags;                 // +0x0c
public:
    void SetTargetingFlags(unsigned long);   // 0x007EA270
};

class CTCCombat { public:                         // interface 0x30
    void RemoveFromCombat();               // 0x00733580
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {
struct CModeStunned : CModeBase {
    void LeafInit();
};
}

void NCreatureMode::CModeStunned::LeafInit()
{
    CTCTargeted* targeted;
    if (ParentThing->GetTC(TCI_TARGETED, &targeted))
        targeted->SetTargetingFlags(4);
    if (ParentThing->HasTC(TCI_COMBAT))
        ((CTCCombat*)&ParentThing->GetTC(TCI_COMBAT))->RemoveFromCombat();
}