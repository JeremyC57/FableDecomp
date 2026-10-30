// NScript::CPersonalScript_GlobalThingsScript::CBowerstoneTeleportPointMarker::Main() -- Bowerstone teleport point marker: wait (one script frame at a time) until the hero comes within the script def's TeleporterActivationDist of the marker, show the teleporting tutorial and wait until it is clicked past, then deactivate the "Hook_BowerstoneTeleportTutorial" quest
// Leaf: ?Main@CBowerstoneTeleportPointMarker@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDF800, 268 bytes; entity-script vtable slot 1 (Main) of 0x012C33A4, the vtable the
//   landed Alloc 0x00CDF750 (CBowerstoneTeleportPointMarker::Alloc) stores. The row is referenced only from that slot.
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CBowerstoneTeleportPointMarker@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
//   (private virtual) is exactly the symbol this source emits; the string "Hook_BowerstoneTeleportTutorial" (0x012C33C0).
// Callees: IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall: Me, hero, distance on the stack),
//   CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 41 MsgIsTutorialClickedPast, 70 GetHero, 118 DisplayTutorial
//   (TUTORIAL_CATEGORY_TELEPORTING = 31, misc_def_types.h), 281 DeactivateQuestLater. Global: pGlobalScriptDef 0x0143E90C.
// Layout: Ego_r.pdb CBowerstoneTeleportPointMarker (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18. CScriptDef::TeleporterActivationDist is a long at retail +0xFB4 (Ego_r +0x1070, 0xBC higher: the
//   Dinkumware vectors before it are 4 bytes larger each; the landed GUI_MeleeBeetles +0xF10 / Ego_r +0xFCC agrees).
// Layout source: retail NScript shared headers (engine/CScriptThing.h, CGameScriptInterfaceBase.h, CEntityScriptBase.h).
// Shape: the landed CBSGateGuard::Main / CKG_Gate::Main flow; the distance is `(float)` of the long def field, pushed
//   before the GetHero call (right-to-left arguments); the quest name is a named
//   CCharString local (built before the call's arguments; no EH frame: Main is not built with /EHsc).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CCPPointerInfo {
public:
    long References;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
enum ETutorialCategory { TUTORIAL_CATEGORY_TELEPORTING = 31 };   // misc_def_types.h
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"

class CScriptDef { public:
    unsigned char _pad[0xfb4];
    long TeleporterActivationDist;                     // +0xFB4
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScript_GlobalThingsScript {
public:
    class CBowerstoneTeleportPointMarker;
};
class CPersonalScript_GlobalThingsScript::CBowerstoneTeleportPointMarker : public CEntityScriptBase {
public:
    CPersonalScript_GlobalThingsScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;              // +0x18
private:
    virtual void Main();                               // slot 1
};
}

void NScript::CPersonalScript_GlobalThingsScript::CBowerstoneTeleportPointMarker::Main()
{
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->TeleporterActivationDist)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Game->DisplayTutorial(TUTORIAL_CATEGORY_TELEPORTING)) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgIsTutorialClickedPast()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    CCharString questName("Hook_BowerstoneTeleportTutorial");
    Game->DeactivateQuestLater(questName, 0);
}