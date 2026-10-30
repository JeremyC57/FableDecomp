// CQS_GuardianTrophyDealerInfoScript::WaitForPieceOver() -- once the dealer's piece is over, wait for the GTDI_Maze entity to die, then deactivate the quest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E27AC0, length 233 bytes. A script thread started from Main (0x00E27920, thread
// name "WaitForPieceOver"); class from vtable 0x012E2018 / factory "QS_GuardianTrophyDealerInfo" (the catalog label
// CQ_GuildTrainingScript::WatchForSparrowKilled is propagated).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CScriptThing 0x004AA840, CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, and through the game script interface (+0x40): NewScriptFrame (slot 7),
// GetThingWithScriptName (slot 72), DeactivateQuestLater (slot 281), GetActiveQuestName (slot 655); the inline
// CScriptThing::IsAlive tests PImp.Data and calls the game thing's IsAlive (slot 75).
// Layout: PieceOver +0x48 (Ego_r.pdb +0x5c - 0x14).
// Shape: two rotated wait loops; the maze handle is a named local destroyed out of line on both exits, and the
// quest-name temporary of DeactivateQuestLater reuses the frame word of the dead "GTDI_Maze" string.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

struct CQS_GuardianTrophyDealerInfoScript : CQuestScriptBase {
    bool PieceOver;                                      // +0x48
    void WaitForPieceOver();
};

void CQS_GuardianTrophyDealerInfoScript::WaitForPieceOver()
{
    while (!PieceOver) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing maze = GSI->GetThingWithScriptName("GTDI_Maze");
    while (maze.IsAlive()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
}