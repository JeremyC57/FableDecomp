// NScript::CV_KnotholeGladeGatesScript::InitialiseVariables() -- reset the gates script's state at the start of Main: the
//   thirteen mission flags +0x48..+0x54, BalvKillCount +0x58 and FirstBalvDead +0x68 (TheGate, the CScriptThing at +0x5c,
//   is left alone)
// Leaf: ?InitialiseVariables@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9FEC0, 48 bytes, plain `ret`.
// Identity: FableWin.pdb names CV_KnotholeGladeGatesScript's public members ControlMainBalverines, DoInitialBalverineAttack,
//   DoMission, DoPanicBanter, EndMission, InitialiseVariables, PhaseOne, WatchForCutscene1, WatchForGates,
//   WatchForTermination -- InitialiseVariables (?InitialiseVariables@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ, void,
//   no arguments) is the only reset-shaped one, and "ResetMissionState" (the catalog label) exists in neither PDB. The
//   stores cover exactly the Ego_r.pdb data members (all but TheGate), and the only caller is the class's Main 0x00E9FCD0
//   (call at 0x00E9FCDE, its first statement), as CQ_ArenaScript::InitialiseVariables is used. The landed source modelled
//   it as CMeleeApprentice::Init with unnamed members.
// Layout: Ego_r.pdb CV_KnotholeGladeGatesScript (128 bytes) - 0x14: MissionSucceeded +0x48 (PDB 92) .. ToldSuccess +0x54
//   (PDB 104), BalvKillCount +0x58 (108), TheGate +0x5c (112), FirstBalvDead +0x68 (124).
// Shape: no frame, one zero register, the stores in retail order (0x48..0x4f, 0x51, 0x53, 0x54, 0x50, 0x52, 0x58, 0x68).
namespace NScript {
class CV_KnotholeGladeGatesScript {
public:
    unsigned char _base[0x48];                     // script base (retail 0x48 bytes)
    bool MissionSucceeded;                         // +0x48
    bool MissionFailed;                            // +0x49
    bool MissionOver;                              // +0x4a
    bool Cutscene1Start;                           // +0x4b
    bool HeroAlertedToProblem;                     // +0x4c
    bool GatesOpened;                              // +0x4d
    bool SaidBefore;                               // +0x4e
    bool HeroKilledAVillager;                      // +0x4f
    bool IntroductionFinished;                     // +0x50
    bool BalvsKilled;                              // +0x51
    bool CutsceneStarting;                         // +0x52
    bool Mentioned;                                // +0x53
    bool ToldSuccess;                              // +0x54
    int  BalvKillCount;                            // +0x58
    unsigned char TheGate[0xc];                    // +0x5c (CScriptThing)
    bool FirstBalvDead;                            // +0x68
    void InitialiseVariables();
};
} // namespace NScript

void NScript::CV_KnotholeGladeGatesScript::InitialiseVariables()
{
    MissionSucceeded = false; MissionFailed = false; MissionOver = false; Cutscene1Start = false;
    HeroAlertedToProblem = false; GatesOpened = false; SaidBefore = false; HeroKilledAVillager = false;
    BalvsKilled = false;
    Mentioned = false;
    ToldSuccess = false;
    IntroductionFinished = false;
    CutsceneStarting = false;
    BalvKillCount = 0;
    FirstBalvDead = false;
}
