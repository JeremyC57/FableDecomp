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

class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                   // out of line 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);  // 0x008AB980
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

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