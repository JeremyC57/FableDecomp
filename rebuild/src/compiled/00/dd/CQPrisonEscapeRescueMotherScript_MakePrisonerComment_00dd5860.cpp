// CQ_PrisonEscapeRescueMotherScript::MakePrisonerComment(const CScriptThing&, const CCharString&) -- unless the last comment is still being spoken, start a one-line conversation in which the prisoner says TEXT_QST_055_PRISONER_<line>; returns whether it did
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD5860, length 225 bytes, `ret 8` (PDB:
// ?MakePrisonerComment@CQ_PrisonEscapeRescueMotherScript@NScript@@QAE_NABVCScriptThing@@ABVCCharString@@@Z).
// Callees: operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`, hidden result in ecx), ~CCharString
// 0x0099EAE0, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0 and operator delete 0x00BFE9BC (the inline null CScriptThing),
// and through the quest's game script interface (+0x40): AddNewConversation (slot 364), AddLineToConversation (slot 366),
// IsConversationActive (slot 368).
// Layout: ConversationIndex +0x74 (Ego_r.pdb - 0x14).
// Shape: arguments evaluate right to left: the null listener CScriptThing() is built first (inline ctor over the
// out-of-line CBaseClass ctor, zeroed counted pointer from the ebp zero register), then the speaker, the `false`,
// and the concatenated line (the interface vtable is fetched before the operator+ call and spilled across it); the new
// handle is stored only after both temporaries are destroyed.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
    CScriptThing() {}                                    // the null thing
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

struct CQ_PrisonEscapeRescueMotherScript : CQuestScriptBase {
    long CurrentTask;                                    // +0x48
    long TasksDone[3];                                   // +0x4c
    long TargetNumber[3];                                // +0x58
    bool HeroAgreesToTask;                               // +0x64
    bool HeroHasFoundWeapons;                            // +0x65
    bool MotherCellDoorOpen;                             // +0x66
    bool MotherFollowing;                                // +0x67
    bool MotherDied;                                     // +0x68
    bool WatchingHero;                                   // +0x69
    bool PrisonCutsceneFinished;                         // +0x6a
    bool RaceStarting;                                   // +0x6b
    long YearsPassedInCell;                              // +0x6c
    long PrisonerConversationIndex;                      // +0x70
    long ConversationIndex;                              // +0x74
    bool MakePrisonerComment(const CScriptThing& prisoner, const CCharString& line);
};

bool CQ_PrisonEscapeRescueMotherScript::MakePrisonerComment(const CScriptThing& prisoner, const CCharString& line)
{
    if (GSI->IsConversationActive(ConversationIndex))
        return false;
    long conversation = GSI->AddNewConversation(prisoner, false, false);
    GSI->AddLineToConversation(conversation, "TEXT_QST_055_PRISONER_" + line, false, prisoner, CScriptThing());
    ConversationIndex = conversation;
    return true;
}