// NScript::CV_KnotholeGladeGatesScript::DoInitialBalverineAttack() -- the first balverine attack on Knothole Glade: spawn the surprise balverine at the first balverine point with the gate horn, wait until it is dead, then (with the hero as target and opinion source if a villager is still outside, else at the hero) wait until the point is off screen and the hero is in control, spawn the persistent murderer balverine with the horn and wait until it is dead
// Grade: DIFFER(1940v1940) -- one instruction placed differently at two sites: retail hoists the murderer balverine's `lea ecx,[esp+68h]` above the balverine-wait loop's terminate `jne` (0x00EA1CB2 / 0x00EA1E41, target 0x00EA1E60 past the exit block's lea) and drops it from the latch IsAlive call; VC7.1 here jumps to the full exit block 0x00EA1E5C and keeps the lea in the latch (same length; every other byte and relocation equal; the chunk 781 0x00D6A560 residue)
// Next idea: the hoist needs the latch fall-through to begin with the lea when VC7.1 compares the two successors; ten spellings of the wait (while / for(;;)+break / if+do-while / == true / by-value + const-ref alias / non-const ref / scoped alias), the arm-1 tail as if(!term), and no /Gr left it unchanged -- try an earlier block-count change (the camera wait or the first-balverine wait), or a different exit shape for the pre-balverine returns
// Leaf: ?DoInitialBalverineAttack@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA1720, 1940 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb name set ?DoInitialBalverineAttack@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ;
//   its only caller is the landed same-TU PhaseOne 0x00EA1630 (call at 0x00EA16BB, between the DoPanicBanter spawn and the two
//   ControlMainBalverines rounds, as PhaseOne's source spells it); literals "KGG_HidingVillager" 0x012EDC68,
//   "KGG_InsideShackMarker" 0x012EDC50, "KGG_FirstBalverinePoint" 0x012EDC38, "KGG_Balvs2" 0x012EDC2C, "KGG_Balv1" 0x012EDC20,
//   "KGG_SurpriseBalverine" 0x012EDA04, "KGG_MurdererBalverine" 0x012EDC08, "KGG_VillagerOutside" 0x012EDA24,
//   "CREATURE_BALVERINE_01" 0x012DFD08, "KNG_HORN" 0x012CB198, "OPINION_SOURCE_INANIMATE_EVIL_HIGH" 0x012CB260.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430 (the expanded scripted-thing
//   handle ctor and the hot-exit dtor), ~CScriptGameResourceObjectScriptedThingBase COMDAT 0x007E74D0 (cold exits), CBaseClass
//   dtor 0x0099A2E0 / operator delete 0x00BFE9BC (the expanded ~CScriptThing), ~CScriptThing 0x004AA840 (cold exits),
//   CScriptThing::operator= 0x008AB980, IsActiveThreadTerminating 0x00CB7940; CScriptThing slot 75 IsAlive through the vptr on
//   the reference-bound balverine; implementation slots 6 GetPos (NullCoord 0x0143E8E0) and 75 IsAlive behind the inline
//   wrappers; CGameScriptInterfaceBase slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 74
//   GetRandomThingWithScriptName, 91 CreateCreature, 92 CreateCreatureNearby, 394 IsHeroControlledByPlayer, 421
//   IsCameraPosOnScreen, 451 GiveThingBestEnemyTarget, 477 SetThingPersistent, 622 EntitySetAsOpinionSource, 623
//   EntityUnsetAsOpinionSource, 689 PlayCriteriaSoundOnThing.
// Layout (retail = Ego_r.pdb - 0x14, as the landed DoPanicBanter 0x00EA1EC0 / PhaseOne): IntroductionFinished +0x50,
//   TheGate +0x5c, FirstBalvDead +0x68.
// Shape: the five marker handles, the unused scripted-thing handle and the surprise balverine live for the whole function
//   (their inline destructors are the hot first-loop exit, the out-of-line chain every other exit); the murderer balverine is
//   bound to a const reference (retail calls IsAlive through its vptr).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
__forceinline const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }
namespace NScript {
class CV_KnotholeGladeGatesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                                  // +0x48
    bool MissionFailed;                                     // +0x49
    bool MissionOver;                                       // +0x4a
    bool Cutscene1Start;                                    // +0x4b
    bool HeroAlertedToProblem;                              // +0x4c
    bool GatesOpened;                                       // +0x4d
    bool SaidBefore;                                        // +0x4e
    bool HeroKilledAVillager;                               // +0x4f
    bool IntroductionFinished;                              // +0x50
    bool BalvsKilled;                                       // +0x51
    bool CutsceneStarting;                                  // +0x52
    bool Mentioned;                                         // +0x53
    bool ToldSuccess;                                       // +0x54
    long BalvKillCount;                                     // +0x58
    CScriptThing TheGate;                                   // +0x5c
    bool FirstBalvDead;                                     // +0x68
    void DoInitialBalverineAttack();
};
}
void NScript::CV_KnotholeGladeGatesScript::DoInitialBalverineAttack()
{
    CScriptGameResourceObjectScriptedThingBase seh_Balverine;
    CScriptThing hidingVillager = Game->GetThingWithScriptName("KGG_HidingVillager");
    CScriptThing insideShack = Game->GetThingWithScriptName("KGG_InsideShackMarker");
    CScriptThing firstPoint = Game->GetThingWithScriptName("KGG_FirstBalverinePoint");
    CScriptThing balvs2 = Game->GetThingWithScriptName("KGG_Balvs2");
    CScriptThing balv1 = Game->GetThingWithScriptName("KGG_Balv1");
    CScriptThing surprise = Game->CreateCreature("CREATURE_BALVERINE_01", firstPoint.GetPos(), "KGG_SurpriseBalverine", false);
    Game->PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
    FirstBalvDead = false;
    while (!FirstBalvDead) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing target = Game->GetRandomThingWithScriptName("KGG_VillagerOutside");
    if (target.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        target = Game->GetHero();
        Game->EntitySetAsOpinionSource(target, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
        while (Game->IsCameraPosOnScreen(firstPoint.GetPos()) || !Game->IsHeroControlledByPlayer()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        const CScriptThing& balv = Game->CreateCreatureNearby("CREATURE_BALVERINE_01", firstPoint.GetPos(), 0.5f, "KGG_MurdererBalverine", false);
        Game->PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
        Game->SetThingPersistent(balv, true);
        Game->GiveThingBestEnemyTarget(balv, target);
        Game->EntitySetAsOpinionSource(balv, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
        IntroductionFinished = true;
        while (balv.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->EntityUnsetAsOpinionSource(target, true);
    } else {
        if (IsActiveThreadTerminating()) return;
        while (Game->IsCameraPosOnScreen(firstPoint.GetPos()) || !Game->IsHeroControlledByPlayer()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        const CScriptThing& balv = Game->CreateCreatureNearby("CREATURE_BALVERINE_01", firstPoint.GetPos(), 0.5f, "KGG_MurdererBalverine", false);
        Game->PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
        Game->SetThingPersistent(balv, true);
        Game->GiveThingBestEnemyTarget(balv, Game->GetHero());
        IntroductionFinished = true;
        while (balv.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
}
