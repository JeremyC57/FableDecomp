// CV_SwordInTheStoneScript::WatchForDeathDuringSwordPull(CScriptThing puller) -- with an is-alive predicate on the crowd member pulling the sword, wait until he is killed or hit by the hero, then take scripting control of him, clear his commands, end the crowd pull, make the sword usable again and restart the crowd timer at 10..15
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED7320, 0x258 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (the predicate's copy), CScriptBase::SetPredicate 0x00CB7920,
//   ~CIsThingAlive 0x00E241B0, IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CBaseClass
//   0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line at the late exits), the scripted-thing
//   handle (CBaseIntelligentPointer ctor 0x0099A380, ClearCommands 0x007E7360, dtor 0x007E74D0), rand 0x00BFEB16;
//   Game slots 7 NewScriptFrame, 8 StartScriptingEntity, 72 GetThingWithScriptName, 447 SetThingAsUsable; the timer
//   through g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer; the thing's implementation slots 18 MsgIsKilledBy,
//   21 MsgIsHitBy, 43 MsgIsHitByAnyAggressiveSpecialAbilityFrom behind the null-safe wrappers.
// Shape: the landed CV_TalentlessBardScript::WatchForAttack 0x00ED9790 predicate/by-value-thing model (b18 recipe:
//   __forceinline ~CScriptThing, in-body inline_depth(0) around the exits retail calls out of line); handle model from
//   the landed CMeleeThunder::Main 0x00D58080.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, CrowdAttemptingToPullSword +0x4d, CrowdSwordPullActive +0x4e,
//   CrowdSwordTimer +0x7c.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
inline bool CScriptThing::MsgIsHitBy(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitBy(attacker); }
inline bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const { if (!PImp.Data) return false; return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker); }
namespace NScript {
class __declspec(novtable) CPredicateBase { public:               // vtable 0x012C2FF4 (never stored: novtable)
    virtual bool IsTrue() = 0;
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:            // vtable 0x012C32F0, 16 bytes
    CScriptThing Thing;                                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    virtual ~CIsThingAlive();                                       // 0x00E241B0
    virtual bool IsTrue();
};
}
using NScript::CIsThingAlive;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
__forceinline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
extern "C" int __cdecl rand();                          // 0x00BFEB16
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_SwordInTheStoneScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    long SwordRemovalAttemptCount;                      // +0x48
    bool SwordCutscene;                                 // +0x4c
    bool CrowdAttemptingToPullSword;                    // +0x4d
    bool CrowdSwordPullActive;                          // +0x4e
    unsigned char _pad4f[0x7c - 0x4f];
    CTimer CrowdSwordTimer;                             // +0x7c
    void WatchForDeathDuringSwordPull(CScriptThing puller);
};
} // namespace NScript
using NScript::CV_SwordInTheStoneScript;

void NScript::CV_SwordInTheStoneScript::WatchForDeathDuringSwordPull(CScriptThing puller)
{
    SetPredicate(CIsThingAlive(puller));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (puller.MsgIsKilledBy("") || puller.MsgIsHitBy("SCRIPT_NAME_HERO") || puller.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            CScriptGameResourceObjectScriptedThingBase handle;
            Game->StartScriptingEntity(puller, handle, (EScriptAIPriority)4);
            handle.ClearCommands();
            CrowdAttemptingToPullSword = false;
            CrowdSwordPullActive = false;
            CScriptThing sword = Game->GetThingWithScriptName("SwordOfStrength");
            Game->SetThingAsUsable(sword, true);
            CrowdSwordTimer = rand() % 6 + 10;
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}