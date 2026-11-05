// NScript::CQ_BanditCampBossBattleScript::CCampHostage2::Main() -- bandit-camp boss battle, the second hostage: hold an alive filter over Me, take Me under scripted
// Leaf: ?Main@CCampHostage2@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
//   control (priority 4, retried each frame); until the hostages are rescued or one is killed, whenever the cry timer has run
//   out and the hero is within 8 units, turn to the hero, play CS_DISAPPOINTED and cry out (alternating the FIRST/SECOND
//   lines in a new conversation with the hero), then rearm the timer for 8 seconds; finally release Me if not terminating
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D08790, 730 bytes (736 with padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (CIsThingAlive filter, vtable 0x012C32F0, inline dtor here),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CBaseObject ctor 0x0099A380,
//   scripted-thing handle (vtable 0x0127094C) IsValid 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation 0x007E73D0 (slot 18)
//   / dtor 0x007E74D0, CTimer dtor 0x00CD4470, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CCharString ctor
//   0x0099EBF0 / dtor 0x0099EAE0; through Game (+4): 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero,
//   364 AddNewConversation, 365 AddPersonToConversation, 366 AddLineToConversation, 475 EntitySetFacingAngleTowardsThing;
//   through the global interface 0x0143E8F8: 87 RegisterTimer, 89 SetTimer, 90 GetTimer. The PlayAnimation flag read from
//   the global byte 0x01375748 (unnamed). Literals "CS_DISAPPOINTED" 0x012C90EC,
//   "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST" 0x012C909C, "TEXT_QST_009_HOSTAGE_SECOND_CRY_SECOND" 0x012C90C4.
// Layout: Ego_r.pdb CCampHostage2 (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18;
//   CQ_BanditCampBossBattleScript HostagesRescued PDB +0x62 / HostageKilled +0x63 = retail +0x4e / +0x4f.
// Naming wave 35: identity reg 00d03c58 "CampHostage2" in 00d038e0; Alloc 00d08670; vtable 012c9080 slot 1; PDB ?Main@CCampHostage2@CQ_BanditCampBossBattleScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CCampHostage2::Main (wave3/labels_alloc.tsv); PDB ?Main@CCampHostage2@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CCampHostage2@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_BanditCampBossBattleScript moved into NScript); same bytes. Also: global 0x01375748 respelled NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations.
namespace NScript { class CPredicateBase; }
#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void f00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748 (established spelling; naming wave 35: was g_PlayAnimationDefaultFlag)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
namespace NScript {
class CQ_BanditCampBossBattleScript {
public:
    char _pad[0x4e];
    bool HostagesRescued;                              // +0x4e
    bool HostageKilled;                                // +0x4f
public:
    class CCampHostage2;             // the entity class below (naming wave 35)
};
}
using NScript::CQ_BanditCampBossBattleScript;
namespace NScript {
class CQ_BanditCampBossBattleScript::CCampHostage2 : public CEntityScriptBase {
public:
    CQ_BanditCampBossBattleScript* ParentClass;        // +0x14
    void*                          PMasterData;        // +0x18 CQ_SunnyvaleMasterData*
private:
    virtual void Main();                          // PDB ?Main@CCampHostage2@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
};
}

// R&D I-3 sweep: a same-TU auto_inline(off) body for the timer dtor 0x00CD4470 (it only reads its own index) lets the
// timer index and the constant 0 stay cached in ebx/ebp across the loop's calls, as in retail.
long UnregisterScriptTimer(long index);
#pragma auto_inline(off)
CTimer::~CTimer()
{
    UnregisterScriptTimer(TimerIndex);
}
#pragma auto_inline(on)
void NScript::CQ_BanditCampBossBattleScript::CCampHostage2::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating()) {
        CTimer cryTimer;
        cryTimer = 0;
        long cry = 0;
        while (!ParentClass->HostagesRescued && !ParentClass->HostageKilled) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (cryTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 8.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                scripted.PlayAnimation("CS_DISAPPOINTED", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                switch (cry) {
                case 0:
                    Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST", false, Me, Game->GetHero());
                    cry = 1;
                    break;
                case 1:
                    Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SECOND_CRY_SECOND", false, Me, Game->GetHero());
                    cry = 0;
                    break;
                }
                cryTimer = 8;
            }
        }
        if (!IsActiveThreadTerminating())
            scripted.Release();
    }
}