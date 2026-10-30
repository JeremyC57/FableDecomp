// NScript::CV_BordelloScript::CBordelloClient::OnPredicateFail() -- a client is gone: one fewer client alive and its slot is free again
// Leaf: ?OnPredicateFail@CBordelloClient@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3B230, 18 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012E3B48, which the landed
//   NScript::CV_BordelloScript::CBordelloClient::Alloc 0x00E3B0F0 stores (site 0x00E3B126); Ego_r.pdb has ?OnPredicateFail@CBordelloClient@CV_BordelloScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CBordelloClient (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CGameScriptInterface;
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BordelloScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool HeroPartying;                                  // +0x48
    bool HeroTricking;                                  // +0x49
    bool MagicianSleeping;                              // +0x4a
    bool CutscenePlaying;                               // +0x4b
    bool PlayerOwned;                                   // +0x4c
    bool BecomeNunnery;                                 // +0x4d
    bool PlayerHasLeftRegion;                           // +0x4e
    bool HeroFoundDeeds;                                // +0x4f
    bool HeroFoundDeedsLocation;                        // +0x50
    long ClientsAlive;                                  // +0x54
    bool ClientInUse[3];                                // +0x58
    class CBordelloClient;
};
class CV_BordelloScript::CBordelloClient : public CEntityScriptBase {
public:
    CV_BordelloScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long BrainState;                                    // +0x1c
    long ClientID;                                      // +0x20
    long SexChance;                                     // +0x24
    long NextLine;                                      // +0x28
private:
    virtual void OnPredicateFail();
};

void CV_BordelloScript::CBordelloClient::OnPredicateFail()
{
    --ParentClass->ClientsAlive;
    ParentClass->ClientInUse[ClientID] = false;
}
}