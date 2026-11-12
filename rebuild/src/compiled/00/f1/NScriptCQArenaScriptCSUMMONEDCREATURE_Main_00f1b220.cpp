// NScript::CQ_ArenaScript::CSUMMONED_CREATURE::Main() -- an arena summoned creature: register a consciousness filter over Me, count it into the arena's creature totals, then every frame score the crowd when the hero hits it (a flourish +5 or a decapitation +7, each also playing the nearest "ArenaSpawn" crowd-loop sound and restarting the crowd timer when that timer allows; any other hit by the hero, or a special ability other than ability 14, +2)
// Leaf: ?Main@CSUMMONED_CREATURE@CQ_ArenaScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line, as retail calls them here). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Timer shape (advisor): the timer goes through inline CTimer accessors: `GetTime()` reads the handle through a
// `const long&` (retail's `add eax,100h ; mov eax,[eax]` before the GetTimer push) and `SetTime()` stores it directly. That
// reference temp is what tips VC7.1's allocator: the conditional-temporary flags word stays in its frame home (immediate
// zero-store at entry, re-read before the hit chain), `bl` goes to the flourish/decapitate results, and ebx/ebp shrink-wrap
// after the first two exits. With a bare `GetTimer(ParentClass->GlobalCrowdTimer.Handle)` the flags take ebx from entry
// (DIFFER 921v907).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F1B220, length 907 bytes, plain `ret`.
// Callees: CIsThingConscious filter (vtable 0x012C2FE8) over the CScriptThing copy ctor 0x004ABE90, its out-of-line dtor
// 0x00EF7AB0, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 / 0x0099EAE0;
// ~CScriptThing 0x004AA840; CScriptThing slots 21 MsgIsHitBy, 22 MsgIsHitByWithFlourish, 23 MsgIsHitByWithDecapitate,
// 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom; CGameScriptInterface slots 7 NewScriptFrame,
// 70 GetHero, 77 GetNearestWithScriptName, 89 SetTimer, 90 GetTimer, 689 PlayCriteriaSoundOnThing (the timer pair through
// pDSTGame 0x0143E8F8). Literals "SCRIPT_NAME_HERO" 0x0125D1C8, "ArenaSpawn" 0x01259BC4.
// Layout: Ego_r.pdb CSUMMONED_CREATURE (28 bytes, the CQ_ArenaScript one): CEntityScriptBase {vptr, Game +4, Me +8},
// ParentClass +0x14, PMasterData +0x18. CQ_ArenaScript is retail = PDB - 0x14 before Rounds (12-byte STLport vector) and
// - 0x18 after it: CrowdLoopTags +0x48, TotalCreatures +0xd0, ExtraCreatures +0xdc, NewCrowdPoints +0xf4,
// NewCrowdBaseLevel +0xfc, GlobalCrowdTimer +0x100.
// Shape: the sibling CRansomRevenge::Main 0x00DE3830 hit chain (conditional string temporaries with flag bits 1, 2, 4,
// here spilled to the frame); the watch loop is a rotated `while (!IsActiveThreadTerminating())`.
// Naming wave 35: identity reg 00f0ff89 "SUMMONED_CREATURE" in 00f0fb70; Alloc 00f1b0f0; vtable 012f9cc8 slot 1; PDB ?Main@CSUMMONED_CREATURE@CQ_ArenaScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CSUMMONED_CREATURE::Main (wave3/labels_alloc.tsv); PDB ?Main@CSUMMONED_CREATURE@CQ_ArenaScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CSUMMONED_CREATURE@@QAEXXZ`; this source now defines the PDB member (outer stand-in CQ_ArenaScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
};
enum EHeroAbility { HERO_ABILITY_14 = 14 };
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
// NScript::CIsThingConscious: the retail model (vtable 0x012C2FE8, Thing +0x04, 0x10); its dtor 0x00EF7AB0 is called out of line
//   (agentNF25b: was a global local model binding ??1CIsThingConscious@@QAE@XZ; Ego_r + FableWin ??1CIsThingConscious@NScript@@UAE@XZ).
#include "engine/CIsThingConscious.h"
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline long CTimer::GetTime() const { const long& handle = TimerIndex; return pDSTGame->GetTimer(handle); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
namespace NScript {
class CQ_ArenaScript {
public:                                // retail = Ego_r.pdb - 0x14 up to Rounds, - 0x18 after it
    unsigned char _pad00[0x48];
    CCharString   CrowdLoopTags[4][5];                 // +0x48
    unsigned char _pad98[0xd0 - 0x98];
    long          TotalCreatures[3];                   // +0xd0
    long          ExtraCreatures;                      // +0xdc
    bool          MissionSucceeded;                    // +0xe0
    bool          MissionFailed;                       // +0xe1
    bool          ArenaSpawnNeeded[16];                // +0xe2
    long          NewCrowdPoints;                      // +0xf4
    long          NewCrowdHappiness;                   // +0xf8
    long          NewCrowdBaseLevel;                   // +0xfc
    CTimer        GlobalCrowdTimer;                    // +0x100
public:
    class CSUMMONED_CREATURE;        // the entity class below (naming wave 35)
};
}
using NScript::CQ_ArenaScript;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
namespace NScript {
class CQ_ArenaScript::CSUMMONED_CREATURE : public CEntityScriptBase {
public:
    CQ_ArenaScript* ParentClass;                       // +0x14
    void*           PMasterData;                       // +0x18 CQ_SunnyvaleMasterData*
private:
    virtual void Main();                          // PDB ?Main@CSUMMONED_CREATURE@CQ_ArenaScript@NScript@@EAEXXZ
};
}

void NScript::CQ_ArenaScript::CSUMMONED_CREATURE::Main()
{
    {
        CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    ++ParentClass->TotalCreatures[0];
    ++ParentClass->ExtraCreatures;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitByWithFlourish("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->GlobalCrowdTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating())
                    return;
                Game->PlayCriteriaSoundOnThing(Game->GetNearestWithScriptName(Game->GetHero(), "ArenaSpawn"),
                                               ParentClass->CrowdLoopTags[ParentClass->NewCrowdBaseLevel][3]);
                ParentClass->GlobalCrowdTimer = 10;
                ParentClass->NewCrowdPoints += 5;
            }
        } else if (Me.MsgIsHitByWithDecapitate("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->GlobalCrowdTimer.GetTime() <= 8) {
                if (IsActiveThreadTerminating())
                    return;
                Game->PlayCriteriaSoundOnThing(Game->GetNearestWithScriptName(Game->GetHero(), "ArenaSpawn"),
                                               ParentClass->CrowdLoopTags[ParentClass->NewCrowdBaseLevel][3]);
                ParentClass->GlobalCrowdTimer = 10;
                ParentClass->NewCrowdPoints += 7;
            }
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                   || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                       && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_14, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->NewCrowdPoints += 2;
        }
        Game->NewScriptFrame();
    }
}