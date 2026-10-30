// CV_FishingCompetitionScript::SuspendCompetition() -- the organiser has run away: stop the competition and, once neither the Fisherman nor the Greatwood Entrance region is loaded, allow it to run again
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E80C00, length 206 bytes. A script thread started twice from 0x00E80060 with
// the name "SuspendCompetition" (PDB: ?SuspendCompetition@CV_FishingCompetitionScript@NScript@@); the catalog label
// CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation is propagated.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0, and through the game script interface (+0x40): IsRegionLoaded (slot 12, +0x30), NewScriptFrame (slot 7).
// Layout (Ego_r.pdb - 0x14): OrganiserRanAway +0x49, CompetitionRunning +0x4b, AttackCounter +0x54, HitCount +0x5c.
// Shape: the `||` of two IsRegionLoaded calls on implicit CCharString temporaries keeps a construction-flag word in ebx
// (bit 0 / bit 1) and materialises the condition into a frame byte before the conditional destructors; that keeps the
// while loop top-tested (no rotation), with the termination test at the bottom.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

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

namespace NScript {
class CV_FishingCompetitionScript : public CQuestScriptBase {
public:
    bool BountyHuntActive;                               // +0x48
    bool OrganiserRanAway;                               // +0x49
    bool OrganiserKilled;                                // +0x4a
    bool CompetitionRunning;                             // +0x4b
    bool CompetitionWon;                                 // +0x4c
    bool RodOfChampsGiven;                               // +0x4d
    bool CommentIRanAway;                                // +0x4e
    bool CompetitorsAttacked;                            // +0x4f
    bool CompetitorsJustGotAttacked;                     // +0x50
    long AttackCounter;                                  // +0x54
    long AttackDayCount;                                 // +0x58
    long HitCount;                                       // +0x5c
    void SuspendCompetition();
};
} // namespace NScript
using NScript::CV_FishingCompetitionScript;

void NScript::CV_FishingCompetitionScript::SuspendCompetition()
{
    OrganiserRanAway = true;
    CompetitionRunning = false;
    AttackCounter += 5;
    HitCount = 0;
    while (GSI->IsRegionLoaded("Fisherman") || GSI->IsRegionLoaded("GreatwoodEntrance")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating())
        CompetitionRunning = true;
}