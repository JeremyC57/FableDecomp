// CV_AmbushScamScript::WatchForTricksterComplete(CScriptThing) -- wait until the trickster's part is complete, then, if he is still alive, clear his has-information marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E32200, length 188 bytes, `ret 0Ch` (the 12-byte CScriptThing by value).
// A script thread started from 0x00E2FF00 with the name "WatchForTricksterComplete" (PDB: CV_AmbushScamScript, factory
// vtable 0x012E286C); the catalog label CQ_GuildTrainingWoodsWillScript::EndMission is propagated.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CScriptThing 0x004AA840 (and inline: vtable 0x01238C8C,
// counted-pointer release with operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0), NewScriptFrame (slot 7) and
// ClearThingHasInformation (slot 361) through the game script interface (+0x40).
// Layout: TricksterComplete +0x56 (Ego_r.pdb +0x6a - 0x14).
// Shape: the by-value trickster has THREE destructor sites (the terminating return inside the wait loop, a second
// `if (IsActiveThreadTerminating()) return;` after it, and the end): VC7.1 expands the inline destructor at the first
// site only and calls the out-of-line ~CScriptThing at the other two, which tail-merge (the two-site `&&` form expands
// both -- the parked 208-byte form).
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
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

namespace NScript {
class CV_AmbushScamScript : public CQuestScriptBase {
public:
    bool QuestFinished;                                  // +0x48
    bool QuestComplete;                                  // +0x49
    bool QuestActivated;                                 // +0x4a
    bool LeftRegion;                                     // +0x4b
    bool MoralitySwitch;                                 // +0x4c
    bool BuddyIntroComplete;                             // +0x4d
    bool DoTricksterIntro;                               // +0x4e
    bool DoAmbush;                                       // +0x4f
    bool TricksterApproachComplete;                      // +0x50
    bool TricksterSpeechComplete;                        // +0x51
    bool TricksterRetreatComplete;                       // +0x52
    bool TricksterQuestComplete;                         // +0x53
    bool TricksterWhiningComplete;                       // +0x54
    bool TricksterDeadOrLeft;                            // +0x55
    bool TricksterComplete;                              // +0x56
    void WatchForTricksterComplete(CScriptThing trickster);
};
} // namespace NScript
using NScript::CV_AmbushScamScript;

void NScript::CV_AmbushScamScript::WatchForTricksterComplete(CScriptThing trickster)
{
    while (!TricksterComplete) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (trickster.IsAlive() && !IsActiveThreadTerminating())
        GSI->ClearThingHasInformation(trickster);
}