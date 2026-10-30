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

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
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