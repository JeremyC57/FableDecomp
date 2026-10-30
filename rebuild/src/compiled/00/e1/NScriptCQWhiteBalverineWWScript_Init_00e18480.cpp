// CQ_WhiteBalverineWWScript::Init() -- register the three regions of the Witchwood White Balverine quest and reset the mission flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E18480, length 236 bytes; vtable 0x012E0CE4 slot 3, factory
// "Q_WhiteBalverineWW" / "S_QWBW" (0x00E1A770). The catalog label CQ_SecretPassageScript::Init is propagated.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 and AddQuestRegion (slot 271, +0x43c)
// through the quest's game script interface (+0x40).
// Layout (Ego_r.pdb - 0x14): MissionFailed +0x48, MissionSucceeded +0x49, WhiteBalverineAlive +0x4a.
// Shape: both string arguments are implicit conversion temporaries built right to left and destroyed in reverse.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
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
class CQ_WhiteBalverineWWScript : public CQuestScriptBase {
public:
    bool MissionFailed;                                  // +0x48
    bool MissionSucceeded;                               // +0x49
    bool WhiteBalverineAlive;                            // +0x4a
    long WhiteBalverineHealth;                           // +0x4c
    CScriptThing WhiteBalverine;                         // +0x50
    void Init();
};
} // namespace NScript
using NScript::CQ_WhiteBalverineWWScript;

void NScript::CQ_WhiteBalverineWWScript::Init()
{
    GSI->AddQuestRegion("Q_WhiteBalverineWW", "KnotholeGlade");
    GSI->AddQuestRegion("Q_WhiteBalverineWW", "Witchwood4");
    GSI->AddQuestRegion("Q_WhiteBalverineWW", "DemonDoor_KnotholeGlade");
    MissionFailed = false;
    MissionSucceeded = false;
    WhiteBalverineAlive = true;
}