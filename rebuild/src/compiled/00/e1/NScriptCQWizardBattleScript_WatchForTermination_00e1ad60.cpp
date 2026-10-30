// CQ_WizardBattleScript::WatchForTermination() -- wait until the mission has failed or succeeded; on failure mark the active quest failed, then deactivate it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1AD60, length 216 bytes. A script thread started from Main (0x00E1A990,
// thread name "WatchForTermination"); class from vtable 0x012E0F54 stored by the landed ctor 0x00E1A800 (the catalog
// label CQ_HangingTreeEvilScript::BanditYells is propagated; twin of CQ_SecretPassageScript::WatchForTermination 0x00DED0C0).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CWideString(const wchar_t*) 0x0099B6B0 / ~CWideString
// 0x0099B510, ~CCharString 0x0099EAE0, and through the game script interface (+0x40): NewScriptFrame (slot 7),
// DeactivateQuestLater (slot 281), SetQuestAsFailed (slot 289), GetActiveQuestName (slot 655, by value).
// Layout: MissionSucceeded +0x4b, MissionFailed +0x4c (Ego_r.pdb - 0x14).
// Shape: the `&&` wait condition rotates into an entry test of both flags and a bottom test of the first; the empty
// wide message is an implicit conversion temporary built before the other arguments, and both GetActiveQuestName
// temporaries share one frame word. Unlike the SecretPassage twin, both arms re-test the thread before the
// deactivation (an if/else with the termination test in each arm).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CWideString { public:                                     // 4 bytes
    void* PStringData;
    CWideString(const unsigned short* text);             // 0x0099B6B0 (wchar_t is unsigned short in the retail build)
    ~CWideString();                                      // 0x0099B510
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
class CQ_WizardBattleScript : public CQuestScriptBase {
public:
    bool VillagersTurnOnHero;                            // +0x48
    bool VillagersChased;                                // +0x49
    bool FightersStart;                                  // +0x4a
    bool MissionSucceeded;                               // +0x4b
    bool MissionFailed;                                  // +0x4c
    bool MissionOver;                                    // +0x4d
    void WatchForTermination();
};
} // namespace NScript
using NScript::CQ_WizardBattleScript;

void NScript::CQ_WizardBattleScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        GSI->SetQuestAsFailed(GSI->GetActiveQuestName(), true, L"", true);
    } else {
        if (IsActiveThreadTerminating())
            return;
    }
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
}