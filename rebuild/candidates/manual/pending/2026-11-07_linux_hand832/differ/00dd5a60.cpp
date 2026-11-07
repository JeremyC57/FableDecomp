// NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main() -- a cell prisoner: until the hero opens his cell door make release / idle comments, on release thank the hero and follow him (persistent, prisoner faction) to the courtyard complaining when hit, then stop region-following and keep commenting on the guards
// Grade: DIFFER(2246v2250) -- 21 lines, per-exit ~CTimer split: retail's GUARDS hit-arm `if (T) return;` exit CALLS ~CTimer out of line (0x00DD6293 jne 0x00DD5B0B, sharing the [call 0x00CD4470; epilogue] tail with the first cell-door exit); ours expands ~CTimer there and cross-jumps it into the function-final block, which then schedules `push ebp` first and absorbs the cell-door exit 0x00DD5DDA
// Next idea: chunk 832: the two GUARDS exits sit in one inliner bucket here (/d2inlT73 outlines both, T74 and up neither; diagnostic only), so retail's source gives the hit-arm exit a lower score than the comment-arm exit; a nested inline `CTimer::operator==` (one or two levels, 1-4 sites) does not shift the bucket, nor do plain `if (T) return;` / `while (!T)` loop tops (58 lines) or a do-while(0) hit-arm T inside `while (!T)` (41). Try a hit test that VC7.1 scores colder: a named bool built by nested ifs, or the comment arm hotter: its T inside the `comment_timer = 30` statement block
// Leaf: ?Main@CPrisoner@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD5A60, 2250 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00DD6329.
// Identity (proven): the binding "Prisoner" in CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0 allocates through the landed
//   CPrisoner::Alloc 0x00DD5940, whose entity vtable slot 1 is this function; Ego_r.pdb
//   ?Main@CPrisoner@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ; Ego_r layout CPrisoner { ParentClass +0x14,
//   PMasterData +0x18, bool ReleasedAndFollowing +0x1c } (struct_layouts_egor.tsv), the TU order Alloc 0x00DD5940 / Init
//   0x00DD59F0 / this, and the comment tags RELEASE / IDLE / COURTYARD / GUARDS of the landed MakePrisonerComment 0x00DD5860.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, rand 0x00BFEB16, ~CScriptThing 0x004AA840,
//   ~CTimer 0x00CD4470, CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, MakePrisonerComment 0x00DD5860; pDSTGame
//   0x0143E8F8 timer slots 87 RegisterTimer / 88 DeregisterTimer / 89 SetTimer / 90 GetTimer; CGameScriptInterfaceBase slots
//   7, 12 IsRegionLoaded, 70 GetHero, 77 GetNearestWithScriptName, 229 EntityFollowThing, 235 EntitySetAsAllowedToFollowHero,
//   364-366 conversations, 368 IsConversationActive, 477 SetThingPersistent, 480 SetEntityAsRegionFollowing, 601
//   EntitySetInFaction; CScriptThing slots 20 MsgIsUsedBy (inline on the cell door), 21 / 41 / 42 (hit tests on Me).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
extern "C" int __cdecl rand();                          // 0x00BFEB16
extern CGameScriptInterfaceBase* pDSTGame;              // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::MsgIsUsedBy(const CCharString& user) const { if (!PImp.Data) return false; return PImp.Data->MsgIsUsedBy(user); }
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };

namespace NScript {
class CQ_PrisonEscapeRescueMotherScript::CPrisoner : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_PrisonEscapeRescueMotherScript&, CScriptDataBase*, const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;
    CQ_PrisonEscapeRescueMotherScript* ParentClass;     // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    bool ReleasedAndFollowing;                          // +0x1c
private:
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    virtual void Main();
};
}

void NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main()
{
    CTimer comment_timer;
    comment_timer = rand() % 90;
    if (!ReleasedAndFollowing) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing cell_door = Game->GetNearestWithScriptName(Me, "CellDoor");
        while (!ReleasedAndFollowing) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (ParentClass->HeroEscapedCell && comment_timer.GetTime() == 0 && ParentClass->MakePrisonerComment(Me, "RELEASE")) {
                if (IsActiveThreadTerminating()) return;
                comment_timer = 30;
            }
            if (!ParentClass->HeroEscapedCell && comment_timer.GetTime() == 0 && ParentClass->MakePrisonerComment(Me, "IDLE")) {
                if (IsActiveThreadTerminating()) return;
                comment_timer = 30;
            }
            if (cell_door.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                while (Game->IsConversationActive(ParentClass->ConversationIndex)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_FREE", false, Me, Game->GetHero());
                ReleasedAndFollowing = true;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
    Game->EntitySetAsAllowedToFollowHero(Me, true);
    Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    Game->SetThingPersistent(Me, true);
    Game->EntitySetInFaction(Me, "FACTION_PRISONER");
    while (!Game->IsRegionLoaded("PrisonCourtyard")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (comment_timer.GetTime() == 0 && ParentClass->MakePrisonerComment(Me, "COURTYARD")) {
            if (IsActiveThreadTerminating()) return;
            comment_timer = 30;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (comment_timer.GetTime() == 0 && ParentClass->MakePrisonerComment(Me, "GUARDS")) {
            if (IsActiveThreadTerminating()) return;
            comment_timer = 30;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_ON_HIT", false, Me, Game->GetHero());
        }
        Game->NewScriptFrame();
    }
}
