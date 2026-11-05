// NCreatureMode::CModeDragonStrafe::OnDeactivate() -- resume a player-type dragon's animation, mark the mode inactive, and set bit 0 of the interface-0x3a component's flag byte again
// Leaf: ?OnDeactivate@CModeDragonStrafe
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC660, 84 bytes (+ int3 padding), `ret`; vtable 0x012686C8 slot 6.
// Callees: CThingAICreature::SuspendAI(bool, long) @ 0x00833A10 (the mode's address is the suspend reason),
//   CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface, type 0x3a = bit 26 of the +0x24 dword).
// Name: FableWin.pdb `?OnDeactivate@CModeDragonStrafe@NCreatureMode@@UAEXXZ` (slot 6).
// Shape: the mirror of CModeDragonStrafe::OnActivate @ 0x008DC600; the inline CModeBase::OnDeactivate (`Active = false`)
//   sits between the resume and the flag set (`or byte ptr [eax+50h],1`).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
struct CThingAnimatable {
};

struct CTCFlagged {                        // the type-0x3a component
    unsigned char _pad00[0x50];
    unsigned char m_flag : 1;              // +0x50 bit 0
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x10];             // WeaponBasedFilter, FramesBeforeRemovingSelf
    bool          Active;                  // +0x18
    void OnDeactivate() { Active = false; }
};

namespace NCreatureMode {
struct CModeDragonStrafe : CModeBase {     // vtable 0x012686C8 (CModeDragonFlyingBase members from +0x1c)
    void OnDeactivate();
};
}

void NCreatureMode::CModeDragonStrafe::OnDeactivate()
{
    if (ParentThing->GetType() == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(false, (void*)((long)this));
    CModeBase::OnDeactivate();
    CTCFlagged* flagged;
    if (ParentThing->GetTC(TCI_MAPWHO, &flagged))
        flagged->m_flag = 1;
}