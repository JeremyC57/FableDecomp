// NCreatureMode::CModeDragonFlying::OnDeactivate() -- make the dragon bloody-willable / liftable again, resume a player creature's animation, clear Active, and set bit 0 of the +0x50 byte of the thing's interface 0x3a component
// Leaf: OnDeactivate
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC590, 104 bytes (`ret`).
// Callees: SetThingAsNotBloodyWillableAtAll(CThing*, bool) @ 0x007E4040 and SetThingIsAbleToBeLifted(CThing*, bool)
// @ 0x007E4330 (both `__fastcall`, the bool in dl), CThingAICreature::SuspendAI @ 0x00833A10,
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x3a: word 1 bit 26 of the presence bitset).
// Layout: CModeBase: ParentThing +4, Active +0x18; the component's flag byte +0x50 bit 0.
// Shape: the GetInterface result IS tested (the absent arm skips the flag store); the parent is reloaded after the
// Active store and `this` is dead from there (esi reused).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CModeBase;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

struct CTCFlagged {                         // the type-0x3a component
    unsigned char _pad00[0x50];
    unsigned char m_flag : 1;               // +0x50 bit 0
};
void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable);   // 0x007E4040
void __fastcall SetThingIsAbleToBeLifted(CThing* thing, bool able);                   // 0x007E4330

struct CModeBase {
    virtual void v00();
    CThing*       ParentThing;              // +0x04
    unsigned char _pad08[0x10];
    bool          Active;                   // +0x18
};

namespace NCreatureMode {

struct CModeDragonFlying : CModeBase {
    virtual void OnDeactivate();
};

void CModeDragonFlying::OnDeactivate()
{
    SetThingAsNotBloodyWillableAtAll(ParentThing, false);
    SetThingIsAbleToBeLifted(ParentThing, true);
    if (ParentThing->GetType() == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(false, this);
    Active = false;
    CTCFlagged* tc;
    if (ParentThing->GetTC(TCI_MAPWHO, &tc))
        tc->m_flag = 1;
}

}