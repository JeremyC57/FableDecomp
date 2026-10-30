// CQ_PrisonRaceScript::MakeRacerComment(const CScriptThing&, const CCharString&) -- unless the last comment is still being spoken, start a one-line conversation in which the racer says TEXT_QST_055_RACER_<line>; returns whether it did
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDCF50, length 225 bytes, `ret 8` (PDB:
// ?MakeRacerComment@CQ_PrisonRaceScript@NScript@@QAE_NABVCScriptThing@@ABVCCharString@@@Z).
// Callees: operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`, hidden result in ecx), ~CCharString
// 0x0099EAE0, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0 and operator delete 0x00BFE9BC (the inline null CScriptThing),
// and through the quest's game script interface (+0x40): AddNewConversation (slot 364), AddLineToConversation (slot 366),
// IsConversationActive (slot 368).
// Layout: ConversationIndex +0x5c (Ego_r.pdb - 0x14).
// Shape: arguments evaluate right to left: the null listener CScriptThing() is built first (inline ctor over the
// out-of-line CBaseClass ctor, zeroed counted pointer from the ebp zero register), then the speaker, the `false`,
// and the concatenated line (the interface vtable is fetched before the operator+ call and spilled across it); the new
// handle is stored only after both temporaries are destroyed.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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

namespace NScript {
class CQ_PrisonRaceScript : public CQuestScriptBase {
public:
    bool HeroWonRace;                                    // +0x48
    bool RaceStarted;                                    // +0x49
    bool RaceEnded;                                      // +0x4a
    long RacerSpeed;                                     // +0x4c
    long HeroWaypointIndex;                              // +0x50
    float NearestRaceMarkerDistanceToHero;               // +0x54
    long NearestRaceMarker;                              // +0x58
    long ConversationIndex;                              // +0x5c
    bool MakeRacerComment(const CScriptThing& racer, const CCharString& line);
};
} // namespace NScript
using NScript::CQ_PrisonRaceScript;

bool NScript::CQ_PrisonRaceScript::MakeRacerComment(const CScriptThing& racer, const CCharString& line)
{
    if (GSI->IsConversationActive(ConversationIndex))
        return false;
    long conversation = GSI->AddNewConversation(racer, false, false);
    GSI->AddLineToConversation(conversation, "TEXT_QST_055_RACER_" + line, false, racer, CScriptThing());
    ConversationIndex = conversation;
    return true;
}