// CQ_HangingTreeEvilScript::BanditYells() -- script thread: each time a bandit has yelled, pause five seconds and clear the flag; otherwise wait a script frame; stops when the script thread is terminating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D69CD0, length 101 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), and through the quest's game script
// interface (+0x40): NewScriptFrame (slot 7, +0x1c), Pause(float) (slot 376, +0x5e0).
// Layout: BanditYelledRecently +0x55 (Ego_r.pdb +0x69 - 0x14).
// Shape: the outer `while (!terminating)` is rotated (entry test, bottom test after the frame); the inner wait loop
// for the flag is the landed EndMission loop; the flag is cleared after the pause.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

struct CQ_HangingTreeEvilScript : CQuestScriptBase {
    bool IntroCutscenePlayed;                               // +0x48
    bool BanditsStarted;                                    // +0x49
    bool BanditKilled;                                      // +0x4a
    bool PartySuccessfullySetUp;                            // +0x4b
    bool MissionSucceeded;                                  // +0x4c
    bool MissionFailed;                                     // +0x4d
    bool BanditsLeftRegion;                                 // +0x4e
    bool SpawnHere;                                         // +0x4f
    bool AtWayPoint;                                        // +0x50
    bool SwitchingRegions;                                  // +0x51
    bool RemoveBandits;                                     // +0x52
    bool AllEnemiesDead;                                    // +0x53
    bool QuestStartScreenShown;                             // +0x54
    bool BanditYelledRecently;                              // +0x55
    void BanditYells();
};

void CQ_HangingTreeEvilScript::BanditYells()
{
    while (!IsActiveThreadTerminating()) {
        while (!BanditYelledRecently) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->Pause(5.0f);
        BanditYelledRecently = false;
        GSI->NewScriptFrame();
    }
}