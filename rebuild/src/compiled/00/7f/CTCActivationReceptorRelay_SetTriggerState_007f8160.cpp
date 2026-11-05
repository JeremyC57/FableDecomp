// CTCActivationReceptorRelay::SetTriggerState(bool state) -- pass the relay's activation state on to the activation-trigger component of its own thing (OnTriggerActivate)
// Leaf: ?SetTriggerState@CTCActivationReceptorRelay@@QAEX_N@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members m_pFirst/m_pLast are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8160, 88 bytes (`ret 4` + int3 padding). FableWin.pdb / Ego_r.pdb
//   `?SetTriggerState@CTCActivationReceptorRelay@@AAEX_N@Z` (private thiscall; modelled public, hence the QAE leaf); callers CTCActivationReceptorRelay::Activate
//   0x007F81C0 (true) and ::Deactivate 0x007F81E0 (false), vtable 0x012414EC slots 28/29. Catalog label
//   `TCInterface_HandleEndCutFade` was a guess (the manifest's "CGameScriptInterface::EndCutFade" on the callers is propagated).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (non-const) @ 0x0040F020 (interface 0x66 = CTCActivationTrigger,
//   presence word 3 bit 6), CTCActivationTrigger::OnTriggerActivate(bool) = slot 28 (+0x70) (vtable 0x0123BBAC slot 28
//   0x0077B060).
// Shape: the PDB's out-parameter `CThing::GetTC<CTCActivationTrigger>(&trigger)` with its result ignored, then the virtual
//   call; a thing without the component calls through the never-written local (homed in the argument slot).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CTCActivationTrigger {
public:
    VF10(0) VF10(1) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    virtual void OnTriggerActivate(bool activate);              // slot 28 (+0x70)
};

class CTCActivationReceptorRelay {
public:
    virtual void v00();
    CThing* ParentThing;                                        // +0x04 (CTCBase)
    void SetTriggerState(bool state);
};

void CTCActivationReceptorRelay::SetTriggerState(bool state)
{
    CTCActivationTrigger* trigger;
    ParentThing->GetTC(TCI_ACTIVATION_TRIGGER, &trigger);
    trigger->OnTriggerActivate(state);
}