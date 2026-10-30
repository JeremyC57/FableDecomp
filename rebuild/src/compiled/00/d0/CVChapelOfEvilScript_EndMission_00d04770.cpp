// CV_ChapelOfEvilScript::EndMission() [propagated label; the body belongs to the bandit-camp quest] -- script thread:
// whenever BanditCampCentre is loaded, open the Gate2Inner door and wait frame by frame until the region unloads
// Leaf: EndMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D04770, 322 bytes (padded to 336).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, ~CScriptThing @ 0x004AA840; CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 12
//          IsRegionLoaded (+0x30), 72 GetThingWithScriptName (+0x120), 425 OpenDoor (+0x6a4).
// Layout: only the interface at +0x40 is used (CQuestScriptBase model as the landed CV_ scripts).
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
    ~CScriptThing();                                     // 0x004AA840 (never expanded in this TU)
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
struct CV_ChapelOfEvilScript : CQuestScriptBase {
    void EndMission();
};

void CV_ChapelOfEvilScript::EndMission()
{
    while (!IsActiveThreadTerminating()) {
        if (GSI->IsRegionLoaded("BanditCampCentre")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing gate = GSI->GetThingWithScriptName("Gate2Inner");
            GSI->OpenDoor(gate);
            while (!(!GSI->IsRegionLoaded("BanditCampCentre"))) {   // WAIT_UNTIL(!loaded)
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        GSI->NewScriptFrame();
    }
}