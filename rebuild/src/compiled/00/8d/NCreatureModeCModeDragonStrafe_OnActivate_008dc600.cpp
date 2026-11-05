// NCreatureMode::CModeDragonStrafe::OnActivate() -- base activation, suspend a player-type dragon's animation for this mode, and clear bit 0 of the interface-0x3a component's flag byte
// Leaf: ?OnActivate@CModeDragonStrafe
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC600, 85 bytes (+ int3 padding), `ret`; vtable 0x012686C8 slot 5.
// Callees: CModeBase::OnActivate @ 0x0073AAC0 (qualified), CThingAICreature::SuspendAI(bool, long) @ 0x00833A10
//   (the mode's address is the suspend reason), CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface,
//   type 0x3a = bit 26 of the +0x24 dword).
// Name: FableWin.pdb `?OnActivate@CModeDragonStrafe@NCreatureMode@@UAEXXZ` (slot 5).
// Shape: as the landed CModeBriarRoseTeleportIntangible::OnActivate @ 0x008E7B90 (same type-0x3a flag clear through a one-bit
//   bitfield: `and byte ptr [eax+50h],0FEh`); ParentThing is re-read for each use.
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
    void OnActivate();                     // 0x0073AAC0
};

namespace NCreatureMode {
struct CModeDragonStrafe : CModeBase {     // vtable 0x012686C8 (CModeDragonFlyingBase members from +0x1c)
    void OnActivate();
};
}

void NCreatureMode::CModeDragonStrafe::OnActivate()
{
    CModeBase::OnActivate();
    if (ParentThing->GetType() == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(true, (void*)((long)this));
    CTCFlagged* flagged;
    if (ParentThing->GetTC(TCI_MAPWHO, &flagged))
        flagged->m_flag = 0;
}