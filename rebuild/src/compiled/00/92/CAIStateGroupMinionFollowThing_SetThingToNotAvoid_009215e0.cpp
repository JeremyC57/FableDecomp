// CAIStateGroup_MinionFollowThing::SetThingToNotAvoid(const CThing* thing) -- tell the minion's navigation component (interface 7) to ignore the given thing when avoiding obstacles
// Leaf: ?SetThingToNotAvoid@CAIStateGroup_MinionFollowThing@@
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x009215E0, 72 bytes (80 with padding), __fastcall `ret 4`.
// Identity: Ego_r.pdb ?SetThingToNotAvoid@CAIStateGroup_MinionFollowThing@@IAEXPBVCThing@@@Z (protected); the only caller
//   is this class's Update (0x009219C9). The catalog label "CAIStateGroup_MinionFollowThing_AssignTask" is an autoname.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (non-const copy),
//   CTCCreatureNavigation::SetThingToIgnore(const CThing*) 0x006CBE30 (manifest "CVillageTaskCarryCrate::AssignCreatureToTask"
//   is propagated; the landed Cleanup 0x00921870 names it).
// Layout: PBrain +0x04 -> PCreature +0x18; the thing's interface bitset at +0x20 (interface 7 = the sign bit of byte +0x20,
//   `test cl,cl ; jns`), the sorted {type, component} vector map at +0x44.
// Shape: the bool out-parameter GetInterface inside the `if` -- the component is stored to the local (`mov eax,[eax+4]`)
//   before the argument push and moved to ecx at the call; the entry-returning FindEntry form folds it straight into ecx
//   (`mov ecx,[eax+4]`). Model shared with the landed CAIStateGroup_MinionFollowThing::Cleanup 0x00921870 (the engine
//   header flattens the base, so the class is modelled locally).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CTCCreatureNavigation { public:                  // interface 7
    void SetThingToIgnore(const CThing* thing); // 0x006CBE30
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
};

struct CAIStateGroup_MinionFollowThing : CAIStateGroupBase {
    void SetThingToNotAvoid(const CThing* thing);
};

void CAIStateGroup_MinionFollowThing::SetThingToNotAvoid(const CThing* thing)
{
    CTCCreatureNavigation* navigation;
    if (PBrain->PCreature->GetTC(TCI_NAVIGATION, &navigation))
        navigation->SetThingToIgnore(thing);
}