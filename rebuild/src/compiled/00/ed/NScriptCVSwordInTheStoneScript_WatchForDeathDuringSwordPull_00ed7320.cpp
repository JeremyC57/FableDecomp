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
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data (CScriptThing vtable layout)
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;             // slot 18 (+0x48)
    virtual void i019(); virtual void i020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;              // slot 21 (+0x54)
    virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029();
    virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037();
    virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const; // slot 43 (+0xac)
    virtual void i044();
    virtual void i045();
    virtual void i046();
    virtual void i047();
    virtual void i048();
    virtual void i049();
    virtual void i050();
    virtual void i051();
    virtual void i052();
    virtual void i053();
    virtual void i054();
    virtual void i055();
    virtual void i056();
    virtual void i057();
    virtual void i058();
    virtual void i059();
    virtual void i060();
    virtual void i061();
    virtual void i062();
    virtual void i063();
    virtual void i064();
    virtual void i065();
    virtual void i066();
    virtual void i067();
    virtual void i068();
    virtual void i069();
    virtual void i070();
    virtual void i071();
    virtual void i072();
    virtual void i073();
    virtual void i074();
    virtual bool IsAlive() const;                                                      // slot 75 (+0x12c)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    __forceinline ~CScriptThing() {}                                      // out of line 0x004AA840
    bool IsAlive() const                                    // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
    bool MsgIsKilledBy(const CCharString& killer) const     // 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
    bool MsgIsHitBy(const CCharString& attacker) const      // 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(attacker);
    }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const   // 0x004AAD60
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
    }
};
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
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
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
            CrowdSwordTimer.SetTime(rand() % 6 + 10);
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}