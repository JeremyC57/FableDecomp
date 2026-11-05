// CCreatureAction_TrollTunnel::PerformAction() -- whack the ground, put the troll into mode 0x1e (tunnelling) through its mode manager (interface type 0x31), then refresh the player GUI
// Leaf: ?PerformAction@CCreatureAction_TrollTunnel@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C63C0, length 97 bytes (+ int3 padding).
// Callees: CCreatureAction_TrollWhackGroundBase::WhackGround @ 0x008C61C0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10 (bool result dropped), CPlayerGui::UpdateMiniMapMarkers(bool) @ 0x004378C0 (landed naming of
// 0x0089EEC0; the manifest "_Cons_val<...>" label is propagated) on the global (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) @ 0x013B8790.
// Shape: the mode manager is an UNINITIALISED out-parameter used unconditionally; here the two paths JOIN (`jmp` to the shared AddMode push) because
// more code follows the call; the miss path reads the never-written frame word into eax. The GUI pointer is loaded straight into ecx after AddMode.
// The PDB header (CCreatureAction_TrollTunnel.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCreatureModeManager { public:             // interface type 0x31 component
    bool AddMode(unsigned long mode);       // 0x0073AE10
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag);   // 0x004378C0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};
class CCreatureAction_TrollWhackGroundBase : public CCombatActionBase { public:
    void WhackGround();                     // 0x008C61C0
};
class CCreatureAction_TrollTunnel : public CCreatureAction_TrollWhackGroundBase { public:
    virtual void PerformAction();
};

void CCreatureAction_TrollTunnel::PerformAction()
{
    WhackGround();
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
    pModes->AddMode(0x1e);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
}