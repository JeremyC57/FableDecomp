// NScript::CV_KnotholeGladeGatesScript::ControlMainBalverines() -- one round of the main balverine attack: pick a random villager outside the gates (the hero if none is alive), wait until it is off screen, spawn a persistent KGG_Balverine next to it with the horn sound, target it at the villager (and make it an evil opinion source), then wait until every spawned balverine is dead
// Grade: DIFFER(2279v1682) -- the two arm-first terminate checks (0x00EA21B9 if-arm, 0x00EA2311 else-arm) jump to the cold tail (~CScriptThing x2 + ~vector out of line) in retail; VC7.1 expands both inline (every other exit, both camera waits, the balverine temporaries and the GetRandom temporary already match; the diagnostic /d2inlT62 reproduces retail's exit pattern, so the two sites are marginal, T* in (62, 74])
// Next idea: lower the frequency of the two arm-first checks only: a form where the if/else arms are reached through one more 50/50 edge (e.g. the alive test as a named bool tested twice, or the arms as two guarded blocks) without changing the single-iteration counter loop; probe/ea2080_c.cpp (do-while counter) is the smallest at 1787
// Leaf: ?ControlMainBalverines@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA2080, 1682 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb name set ?ControlMainBalverines@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ;
//   called twice by the landed same-TU PhaseOne 0x00EA1630 (its two rounds) and nowhere else; literals "KGG_Balv1"
//   0x012EDC20, "KGG_VillagerOutside" 0x012EDA24, "KGG_Balverine" 0x012EDCA0, "CREATURE_BALVERINE_01" 0x012DFD08,
//   "KNG_HORN" 0x012CB198, "OPINION_SOURCE_INANIMATE_EVIL_HIGH" 0x012CB260.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0 / operator delete
//   0x00BFE9BC (the expanded CScriptThing ctor / dtors), ~CScriptThing 0x004AA840 (the cold exits and the spawned-balverine
//   temporaries), vector<CScriptThing>::push_back 0x008ADF90 and ~vector 0x008AC970 (out of line at the cold exits; the
//   element loop + free 0x00BFEA14 expanded at the hot ones), AreAllThingsInVectorDead 0x00CBED00 (__fastcall),
//   IsActiveThreadTerminating 0x00CB7940; CScriptThing implementation slots 6 GetPos (NullCoord 0x0143E8E0 when null) and 75
//   IsAlive behind the inline wrappers; CGameScriptInterfaceBase slots 7 NewScriptFrame, 70 GetHero (reference), 72
//   GetThingWithScriptName, 74 GetRandomThingWithScriptName (by value), 92 CreateCreatureNearby (by value), 421
//   IsCameraPosOnScreen, 451 GiveThingBestEnemyTarget, 477 SetThingPersistent, 622 EntitySetAsOpinionSource, 689
//   PlayCriteriaSoundOnThing.
// Layout (retail = Ego_r.pdb - 0x14, as the landed DoPanicBanter 0x00EA1EC0): TheGate +0x5c.
// Shape: the vector is declared first (its zero stores open the function), then the unused KGG_Balv1 handle and the target;
//   the one-round `for` loop carries the terminate check at its head; the random villager is a by-value temporary assigned
//   through the inline CScriptThing::operator= (ShareData reads Info before Data); each arm's spawned balverine is its own
//   local whose out-of-line destructor the two arms share.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
#include "stlport/fable_containers.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
bool FABLE_FASTCALL AreAllThingsInVectorDead(const std::vector<CScriptThing>& things);   // 0x00CBED00
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
    void ControlMainBalverines();
};
}
void NScript::CV_KnotholeGladeGatesScript::ControlMainBalverines()
{
    std::vector<CScriptThing> balverines;
    CScriptThing balv1 = Game->GetThingWithScriptName("KGG_Balv1");
    CScriptThing target;
    for (long i = 0; i < 1; ++i) {
        if (IsActiveThreadTerminating()) return;
        target = Game->GetRandomThingWithScriptName("KGG_VillagerOutside");
        if (target.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            while (Game->IsCameraPosOnScreen(balv1.GetPos())) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            CScriptThing balv = Game->CreateCreatureNearby("CREATURE_BALVERINE_01", balv1.GetPos(), 6.0f, "KGG_Balverine", false);
            Game->PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
            Game->SetThingPersistent(balv, true);
            Game->GiveThingBestEnemyTarget(balv, target);
            Game->EntitySetAsOpinionSource(balv, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
            balverines.push_back(balv);
        } else {
            if (IsActiveThreadTerminating()) return;
            target = Game->GetHero();
            while (Game->IsCameraPosOnScreen(balv1.GetPos())) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            CScriptThing balv = Game->CreateCreatureNearby("CREATURE_BALVERINE_01", balv1.GetPos(), 6.0f, "KGG_Balverine", false);
            Game->PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
            Game->SetThingPersistent(balv, true);
            Game->GiveThingBestEnemyTarget(balv, target);
            balverines.push_back(balv);
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (!AreAllThingsInVectorDead(balverines)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
}
