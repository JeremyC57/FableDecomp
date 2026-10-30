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