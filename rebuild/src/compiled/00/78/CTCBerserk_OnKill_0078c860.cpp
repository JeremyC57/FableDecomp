// CTCBerserk::OnKill() -- on kill, restart the creature's current mode
// Leaf: ?OnKill@CTCBerserk@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078C860, 76 bytes, thiscall, plain `ret`.
// Identity (proven): slot 14 (OnKill, CTCBase order) of vtable 0x0126BB94 (RTTI .?AVCTCBerserk@@); Ego_r.pdb ?OnKill@CTCBerserk@@UAEXXZ; restarts the creature mode manager's current mode (interface 0x31, bound ?RestartCurrentMode@CTCCreatureModeManager@@QAEXXZ).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline CThing::GetInterface, interface 0x31),
//   CTCCreatureModeManager::RestartCurrentMode() 0x0073A840.
// Shape (family M169, representative 0x0061A360): `CTCCreatureModeManager* tc; ParentThing->GetInterface(&tc, 0x31); tc->RestartCurrentMode();` --
//   the GetInterface result is not tested (an absent component calls through the uninitialised `push ecx` word, as retail
//   does); the GetInterface of the landed CTCWillLightning::StartLoopingSound 0x007A0400. Speed TU.
#pragma warning(disable: 4700)
class CTCBase;
class CTCCreatureModeManager {
public:
    void RestartCurrentMode();                                   // 0x0073A840
};
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBerserk {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    CThing* ParentThing;                                  // +0x04 (CTCBase)
    virtual void OnKill();                              // slot 14
};
void CTCBerserk::OnKill()
{
    CTCCreatureModeManager* tc;
    ParentThing->GetTC(TCI_ENTITY_MODE_MANAGER, &tc);
    tc->RestartCurrentMode();
}