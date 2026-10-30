// CQ_HobbeCaveScript::MakeBanditComment(const CCharString&, const CScriptThing&) -- unless the comment timer is still running, restart it and play one conversation line from the bandit to the hero; returns whether a comment was made
// Leaf: ?MakeBanditComment@CQ_HobbeCaveScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D905C0, length 163 bytes, `ret 8` (PDB:
// ?MakeBanditComment@CQ_HobbeCaveScript@NScript@@QAE_NABVCCharString@@ABVCScriptThing@@@Z); 11 callers in the Hobbe Cave
// bandit scripts.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; the global script interface g_pGameScriptInterface
// (0x0143E8F8) SetTimer (slot 89) / GetTimer (slot 90) through the inline CTimer; the quest's interface (+0x40) GetHero
// (slot 70), AddNewConversation (slot 364), AddPersonToConversation (slot 365), AddLineToConversation (slot 366).
// Data: the script def pGlobalScriptDef (0x0143E90C) HC_BanditCommentTime at +0xcc0 (Ego_r.pdb +0xd70, less four 16-to-12
// byte containers and the 4-byte def base).
// Layout: BanditComment (CTimer) +0x54 (Ego_r.pdb +0x68 - 0x14).
// Shape: the running-timer exit is the usual yield test whose two `return false` arms merge (the thread test is called
// and its result dropped); the conversation handle stays in edi, the interface vtable in ebp per call; arguments
// evaluate right to left, so GetHero() runs before each push of the handle.
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

extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

class CScriptDef { public:
    unsigned char _pad[0xcc0];
    long HC_BanditCommentTime;                           // +0xcc0
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }

namespace NScript {
class CQ_HobbeCaveScript : public CQuestScriptBase {
public:
    bool DoneIntro;                                      // +0x48
    bool SavedBeforeCaves;                               // +0x49
    bool BratInCutscene;                                 // +0x4a
    bool MissionSucceeded;                               // +0x4b
    bool MissionFailed;                                  // +0x4c
    bool AlarmTriggered;                                 // +0x4d
    bool KilledSideRoomHobbes;                           // +0x4e
    bool ReleaseBrat;                                    // +0x4f
    bool BratReturned;                                   // +0x50
    CTimer BanditComment;                                // +0x54
    bool MakeBanditComment(const CCharString& line, const CScriptThing& bandit);
};
} // namespace NScript
using NScript::CQ_HobbeCaveScript;

bool NScript::CQ_HobbeCaveScript::MakeBanditComment(const CCharString& line, const CScriptThing& bandit)
{
    if (BanditComment.GetTime() > 0) {
        if (IsActiveThreadTerminating())
            return false;
        return false;
    }
    BanditComment = pGlobalScriptDef->HC_BanditCommentTime;
    long conversation = GSI->AddNewConversation(bandit, false, false);
    GSI->AddPersonToConversation(conversation, GSI->GetHero());
    GSI->AddLineToConversation(conversation, line, false, bandit, GSI->GetHero());
    return true;
}