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

class CTimer { public:                                          // 4 bytes: an index into the global script timers
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};

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
    BanditComment.SetTime(pGlobalScriptDef->HC_BanditCommentTime);
    long conversation = GSI->AddNewConversation(bandit, false, false);
    GSI->AddPersonToConversation(conversation, GSI->GetHero());
    GSI->AddLineToConversation(conversation, line, false, bandit, GSI->GetHero());
    return true;
}