// NScript::CV_LostTraderScript::CLostTraderTrader::Init() -- the lost trader: brain state 1, comment counters cleared; allowed to follow the hero, scared, an optional-quest minimap marker, region-following the hero without changing that state
// Leaf: ?Init@CLostTraderTrader@CV_LostTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA35A0, 150 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012EDDA8, which the landed
//   NScript::CV_LostTraderScript::CLostTraderTrader::Alloc 0x00EA34F0 stores (site 0x00EA3526); Ego_r.pdb has ?Init@CLostTraderTrader@CV_LostTraderScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CLostTraderTrader (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 235 EntitySetAsAllowedToFollowHero, 496 EntitySetAsScared, 350 MiniMapAddMarker
//   ("HUD_ORB_QUEST_OPTIONAL" temporary), 70 GetHero, 480 SetEntityAsRegionFollowing, 236
//   EntitySetAsAllowedToChangeRegionFollowingState; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
// Shape: the outer call's vtable is loaded (ebx) before the inner GetHero call.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_LostTraderScript {
public:
    class CLostTraderTrader;
};
class CV_LostTraderScript::CLostTraderTrader : public CEntityScriptBase {
public:
    CV_LostTraderScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long stateBrain;                                    // +0x1c
    long YellForHelp;                                   // +0x20
    long YellAtHero;                                    // +0x24
    long RandomComment;                                 // +0x28
private:
    virtual void Init();
};

void CV_LostTraderScript::CLostTraderTrader::Init()
{
    stateBrain = 1;
    YellForHelp = 0;
    YellAtHero = 0;
    RandomComment = 0;
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    Game->EntitySetAsScared(Me, true);
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
    Game->EntitySetAsAllowedToChangeRegionFollowingState(Me, false);
}
}