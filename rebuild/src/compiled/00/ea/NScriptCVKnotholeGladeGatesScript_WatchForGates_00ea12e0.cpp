// CV_KnotholeGladeGatesScript::WatchForGates() -- script thread: every frame, a close request in the shared quest data closes TheGate and an open request opens it (each request cleared after use); ends when the script thread terminates
// Leaf: WatchForGates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA12E0, length 118 bytes (padded to 128), `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (x4), through Game at +0x40 (vtable 0x01260F0C):
// CloseDoor (slot 426, +0x6a8), OpenDoor (slot 425, +0x6a4), NewScriptFrame (slot 7, +0x1c).
// Layout (retail = Ego_r.pdb CV_KnotholeGladeGatesScript - 0x14): Game +0x40, Data +0x44 (the shared quest data:
// close request +0x76, open request +0x77), TheGate (CScriptThing) +0x5c.
// Shape: the rotated `while (!terminating)` loop; `false` lives in bl for the flag tests and clears.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CScriptThing { public:                              // 12 bytes (vtable 0x01238C8C)
    void* __vftable;
    long  m_data[2];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CKnotholeGladeQuestData {
    unsigned char _pad00[0x76];
    bool CloseGatesRequested;                      // +0x76
    bool OpenGatesRequested;                       // +0x77
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_KnotholeGladeGatesScript : public CScriptBase {
public:
    CGameScriptInterface*    Game;                 // +0x40
    CKnotholeGladeQuestData* Data;                 // +0x44
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
    long BalvKillCount;                            // +0x58
    CScriptThing TheGate;                          // +0x5c
    bool FirstBalvDead;                            // +0x68
    void WatchForGates();
};
} // namespace NScript
using NScript::CV_KnotholeGladeGatesScript;
void NScript::CV_KnotholeGladeGatesScript::WatchForGates()
{
    while (!IsActiveThreadTerminating())
    {
        if (Data->CloseGatesRequested)
        {
            if (IsActiveThreadTerminating())
                return;
            Game->CloseDoor(TheGate);
            Data->CloseGatesRequested = false;
        }
        if (Data->OpenGatesRequested)
        {
            if (IsActiveThreadTerminating())
                return;
            Game->OpenDoor(TheGate);
            Data->OpenGatesRequested = false;
        }
        Game->NewScriptFrame();
    }
}