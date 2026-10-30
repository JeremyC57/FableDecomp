// NScript::CV_FishermanScript::CFishermanBulletinBoard::Main() -- the fisherman's bulletin board reads 'wasp plague' until the hero has saved the fisherman from the wasps, then 'come fishing'
// Leaf: ?Main@CFishermanBulletinBoard@CV_FishermanScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7BCA0, 150 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): slot 1 (Main) of the RTTI-less entity vtable 0x012EA108, which the landed
//   NScript::CV_FishermanScript::CFishermanBulletinBoard::Alloc 0x00E7BBE0 stores (site 0x00E7BC16); Ego_r.pdb has ?Main@CFishermanBulletinBoard@CV_FishermanScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CFishermanBulletinBoard (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: interface slots 714 SetReadableObjectTextTag (CCharString temporaries), 7 NewScriptFrame;
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; IsActiveThreadTerminating 0x00F35B30.
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
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
class CV_FishermanScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool IntroPlayed;                                   // +0x48
    bool HeroHasSavedFishermanFromWasps;                // +0x49
    long WaspsAlive;                                    // +0x4c
    bool FishermanMustRunAway;                          // +0x50
    bool QuestCardGiven;                                // +0x51
    long FishermanTasks;                                // +0x54
    class CFishermanBulletinBoard;
};
class CV_FishermanScript::CFishermanBulletinBoard : public CEntityScriptBase {
public:
    CV_FishermanScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Main();
};

void CV_FishermanScript::CFishermanBulletinBoard::Main()
{
    Game->SetReadableObjectTextTag(Me, "TEXT_QST_023_SIGN_WASP_PLAGUE");
    while (!ParentClass->HeroHasSavedFishermanFromWasps) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetReadableObjectTextTag(Me, "TEXT_QST_023_SIGN_COME_FISHING");
}
}