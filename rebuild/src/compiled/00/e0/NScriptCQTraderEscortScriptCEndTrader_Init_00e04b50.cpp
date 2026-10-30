// NScript::CQ_TraderEscortScript::CEndTrader::Init() -- the end trader cannot start the ending yet and is unpushable; once the intro is over he gets a quest minimap marker
// Leaf: ?Init@CEndTrader@CQ_TraderEscortScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E04B50, 114 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012DF748, which the landed
//   NScript::CQ_TraderEscortScript::CEndTrader::Alloc 0x00E04A30 stores (site 0x00E04A66); Ego_r.pdb has ?Init@CEndTrader@CQ_TraderEscortScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CEndTrader (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("HUD_ORB_QUEST_CORE" 0x01258AB0); interface
//   slots 350 MiniMapAddMarker, 844 SetIsPushableByHero.
// Shape: the by-value CScriptThing argument of SetIsPushableByHero is built in its stack slot by the inline copy
//   (CBaseClass vtable 0x01231710, then CScriptThing's 0x01238C8C, PImp add-ref).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderEscortScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long CommentTimer;                                  // +0x48 (CTimer)
    long BalverinesToSurpriseHeroNeeded;                // +0x4c
    long TradersStillAliveCounter;                      // +0x50
    bool InfectedTraderCanGetUp;                        // +0x54
    bool TradersShouldBeScared;                         // +0x55
    bool IntroFinished;                                 // +0x56
    class CEndTrader;
};
class CQ_TraderEscortScript::CEndTrader : public CEntityScriptBase {
public:
    CQ_TraderEscortScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    bool EndingCanStart;                                // +0x1c
private:
    virtual void Init();
};

void CQ_TraderEscortScript::CEndTrader::Init()
{
    EndingCanStart = false;
    Game->SetIsPushableByHero(Me, false);
    if (ParentClass->IntroFinished)
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
}
}