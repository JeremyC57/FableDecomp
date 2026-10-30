// CV_LostTraderScript::WatchForQuestFailureState() [propagated label; the body is a secret-passage quest ender] --
// wait until the success (+0x52) or failure (+0x53) flag is set; on success complete the active quest and hand over
// the Q_SecretPassage card, otherwise fail it; then deactivate the active quest
// Leaf: WatchForQuestFailureState
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0C350, 342 bytes (padded to 352), plain `ret` (no handle parameter).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CWideString(const wchar_t*) @ 0x0099B6B0 / ~
//          @ 0x0099B510, CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; CGameScriptInterface slots 7
//          NewScriptFrame (+0x1c), 281 DeactivateQuestLater (+0x464), 288 SetQuestAsCompleted (+0x480), 289
//          SetQuestAsFailed (+0x484), 295 GiveHeroQuestCardDirectly (+0x49c), 655 GetActiveQuestName (+0xa3c).
// Layout: interface at +0x40; bools +0x52 (succeeded) and +0x53 (failed), names unknown: CV_LostTraderScript ends at
//          +0x58 with no bools there, so the real owner is another quest script.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CWideString { public:                                     // 4 bytes
    void* PStringData;
    CWideString(const unsigned short* text);             // 0x0099B6B0
    ~CWideString();                                      // 0x0099B510
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
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
struct CV_LostTraderScript : CQuestScriptBase {
    unsigned char _pad48[0x52 - 0x48];
    bool          Succeeded;                             // +0x52
    bool          Failed;                                // +0x53
    void WatchForQuestFailureState();
};

void CV_LostTraderScript::WatchForQuestFailureState()
{
    while (!Failed && !Succeeded) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (Succeeded) {
        if (IsActiveThreadTerminating())
            return;
        GSI->SetQuestAsCompleted(GSI->GetActiveQuestName(), false, false, false);
        GSI->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_PRISON_SECRET_PASSAGE", "Q_SecretPassage", false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        GSI->SetQuestAsFailed(GSI->GetActiveQuestName(), false, L"", true);
    }
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
}