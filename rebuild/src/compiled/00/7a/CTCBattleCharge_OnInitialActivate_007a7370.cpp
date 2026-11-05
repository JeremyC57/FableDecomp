// CTCBattleCharge::OnInitialActivate() -- give the owner's creature mode manager (interface 0x31) mode 0x23 when it has one; then, for a live player creature (thing type 1), suspend the owner's animation, otherwise reset the charge
// Leaf: OnInitialActivate
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A7370, length 114 bytes (+14 int3 padding). Vtable slot 6 of CTCBattleCharge
// (0x0126CBFC; catalog "vfunc_6"; slot 6 is the CTCBase OnInitialActivate slot).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface), CTCCreatureModeManager::AddMode
// @ 0x0073AE10 (result dropped), the owner's suspend-animation setter @ 0x00833A10 (catalog "CThingAnimatable::SetSuspend
// Animation", called with true, false), the charge's own reset @ 0x007A7000 (catalog "CTCAssassinRush::UpdateDirectionOverride").
// Shape: the owner in edi for both tests (ParentThing re-read for the setter call); the GetInterface result is used straight from
// the iterator; `mov eax,1` shared by the dead-bit test, the type compare and the `true` push; separate returns per arm.
// Layout: retail CThing: +0x10 thing type (1 = player creature), +0x20 component bitset (0x31 = bit 17 of the dword at +0x24),
// +0x44 interface map, +0x91 bit 0 Dead.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};

class CTCCreatureModeManager { public:
    bool AddMode(unsigned long mode);           // 0x0073AE10
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCBattleCharge : public CTCBase {
public:
    virtual void OnInitialActivate();
    void Reset_007a7000();                               // 0x007A7000
};

void CTCBattleCharge::OnInitialActivate()
{
    CThing* thing = ParentThing;
    CTCCreatureModeManager* modes;
    if (thing->GetTC(TCI_ENTITY_MODE_MANAGER, &modes))
        modes->AddMode(0x23);
    if (!thing->IsDead() && thing->GetType() == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(true, (void*)false);
    else
        Reset_007a7000();
}