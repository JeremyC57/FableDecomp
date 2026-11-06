// NScript::CV_BanditTollScript::CTauntingBandit::Main() -- a taunting toll bandit: with the alive predicate, each frame flee to M_BanditEscapePoint and fade out once the bandits flee, follow the hero in FACTION_BANDITS once they are angered, and while the toll is paid warn the threatening hero (WARNING_10/20/30, then ANGERED_10 angers the bandits) or calm down and taunt him at random when he is near
// Grade: DIFFER(2424v2084) -- per-exit inline split of ~CTimer 0x00CD4470 (and ~CScriptThing 0x004AA840 / the scripted-thing handle dtor 0x007E74D0): with the loop-head macro and the TollPaid-block checks wrapped, the final exit and every TollPaid-block exit match retail's expand/call pattern, but the BanditsFlee and BanditsAngered blocks stay one notch too hot (VC7.1 here expands ~CTimer at the distance-loop exits 0x00E49219/0x00E492A0 and the angered post-wait exit 0x00E49356, and all three dtors at the wait-loop exits 0x00E49131/0x00E49252, where retail calls them); TimerIndex is also cached in ebp. The smallest byte diff is the plain-while form with only the TollPaid checks wrapped (attempt e, DIFFER(2276v2084), 554 lines), which gets the final exit wrong.
// Next idea: find a retail-plausible construct that cools the first two blocks (wrapping only heats them): e.g. the flee/angered handling in its own scope or a guarded block whose static frequency is below the loop body's, or the timer declared in a nested scope around the TollPaid block; meter each exit with a marker callee (FINDINGS.md method).
// Leaf: ?Main@CTauntingBandit@CV_BanditTollScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E49020, 2084 bytes, thiscall, plain `ret` (0x00E49819), then a 24-byte exit tail and the
//   4-entry AngerCount jump table (0x00E49834: 0x00E4950D, 0x00E4954C, 0x00E4958B, 0x00E495CA).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Main@CTauntingBandit@CV_BanditTollScript@NScript@@EAEXXZ; slot 1 of the
//   CTauntingBandit vtable 0x012E4DD4 (the landed Init 0x00E48EB0 is slot 2); TEXT_QST_011_BANDIT_* literals, "M_BanditEscapePoint".
// Callees: SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CScriptThing copy 0x004ABE90 / dtor 0x004AA840,
//   ~CIsThingAlive 0x00E241B0, CBaseObject 0x0099A380 / ~CBaseObject 0x0099A430, IsValid 0x00CD23B9, Release 0x00CD2770,
//   MoveToPosition 0x007E72F0, FollowThing 0x007E7320, IsPerformingScriptTask 0x007E7450, GetScriptThing 0x007E7490, handle dtor
//   0x007E74D0, ~CTimer 0x00CD4470, CCharString 0x0099EBF0 / 0x0099EAE0, IsDistanceFromThingToPositionOver 0x00CBE45C,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity 0x00CBE716, IsPlayerThreateningEntity 0x00CBEED2 (all __fastcall),
//   rand 0x00BFEB16; CScriptThing slots 16 IsAwareOfHero, 75 IsAlive; Game slots 7, 8, 70, 72, 364-366, 475
//   EntitySetFacingAngleTowardsThing, 601 EntitySetInFaction; pDSTGame 0x0143E8F8 slots 87-90 (timer).
// Data: pGlobalScriptDef 0x0143E90C: TollBanditPayDistance +0x414, TollBanditTauntTimer +0x420, TollBanditCloseToExit +0x43C,
//   TollBanditTimeToFade +0x440 (Ego_r CScriptDef +0x448/+0x454/+0x470/+0x474; the retail def is 0x34 lower here, as the landed
//   Init's BanditTollBanditWanderDistance). Parent flags BanditsAngered +0x48, TollPaid +0x49, BanditsFlee +0x4F.
// Inline-split: the loop-head `do { if (IsActiveThreadTerminating()) return; } while (0);` and the wrapped TollPaid-block checks are
//   frequency levers (INLINE_DEPTH_REVISIT.md levers 3/4); retail's construct is unproven.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include <stdlib.h>
#include "engine/CV_BanditTollScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/C3DVector.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_WALK=0, SCRIPT_ENTITY_MOVE_RUN=1 };
class CScriptDef { public:
    unsigned char _pad000[0x414];
    long TollBanditPayDistance;                         // +0x414 (PDB +0x448)
    unsigned char _pad418[0x420 - 0x418];
    long TollBanditTauntTimer;                          // +0x420 (PDB +0x454)
    unsigned char _pad424[0x43c - 0x424];
    float TollBanditCloseToExit;                        // +0x43C (PDB +0x470)
    float TollBanditTimeToFade;                         // +0x440 (PDB +0x474)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C
extern CGameScriptInterfaceBase* pDSTGame;              // 0x0143E8F8
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline const C3DVector& CScriptThing::GetPos() const
{
    if (!PImp.Data) return NullCoord;
    return PImp.Data->GetPos();
}
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing1, const CScriptThing& thing2, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);  // 0x00CBE716
int __fastcall IsPlayerThreateningEntity(const CScriptThing& thing);                         // 0x00CBEED2

void NScript::CV_BanditTollScript::CTauntingBandit::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CScriptThing escapePoint = Game->GetThingWithScriptName("M_BanditEscapePoint");
    CTimer tauntTimer;
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (ParentClass.BanditsFlee) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            float closeToExit = pGlobalScriptDef->TollBanditCloseToExit;
            C3DVector escapePos = escapePoint.GetPos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), escapePos, closeToExit + 2.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                seh_me.MoveToPosition(escapePos, closeToExit, SCRIPT_ENTITY_MOVE_RUN, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, true, pGlobalScriptDef->TollBanditTimeToFade, true);
        }
        if (ParentClass.BanditsAngered && Me.IsAlive() && Me.IsAwareOfHero()) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetInFaction(Me, "FACTION_BANDITS");
            seh_me.FollowThing(Game->GetHero(), 1.0f, true);
        }
        if (ParentClass.TollPaid && !ParentClass.BanditsAngered) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            if (tauntTimer.GetTime() == 0) {
                do { if (IsActiveThreadTerminating()) return; } while (0);
                if (IsPlayerThreateningEntity(Me)) {
                    do { if (IsActiveThreadTerminating()) return; } while (0);
                    if (!AngryWithHero) {
                        do { if (IsActiveThreadTerminating()) return; } while (0);
                        AngryWithHero = true;
                        if (seh_me.IsValid()) seh_me.Release();
                        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
                            Game->NewScriptFrame();
                            do { if (IsActiveThreadTerminating()) return; } while (0);
                        }
                        do { if (IsActiveThreadTerminating()) return; } while (0);
                    }
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                    tauntTimer = pGlobalScriptDef->TollBanditTauntTimer;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    switch (AngerCount) {
                    case 0:
                        Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_WARNING_10", false, Me, Game->GetHero());
                        break;
                    case 1:
                        Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_WARNING_20", false, Me, Game->GetHero());
                        break;
                    case 2:
                        Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_WARNING_30", false, Me, Game->GetHero());
                        break;
                    case 3:
                        Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_ANGERED_10", false, Me, Game->GetHero());
                        ParentClass.BanditsAngered = true;
                        break;
                    }
                    ++AngerCount;
                } else {
                    do { if (IsActiveThreadTerminating()) return; } while (0);
                    if (AngryWithHero) {
                        do { if (IsActiveThreadTerminating()) return; } while (0);
                        long conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_NO_LONGER_THREATENING_10", false, Me, Game->GetHero());
                        AngryWithHero = false;
                        if (seh_me.IsValid()) seh_me.Release();
                    }
                    if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->TollBanditPayDistance)) {
                        do { if (IsActiveThreadTerminating()) return; } while (0);
                        if (rand() % 300 == 0) {
                            do { if (IsActiveThreadTerminating()) return; } while (0);
                            long conversation = Game->AddNewConversation(Me, false, false);
                            Game->AddPersonToConversation(conversation, Game->GetHero());
                            Game->AddLineToConversation(conversation, "TEXT_QST_011_BANDIT_RANDOM_TAUNT", false, Me, Game->GetHero());
                        }
                    }
                }
            }
        }
        Game->NewScriptFrame();
    }
}
