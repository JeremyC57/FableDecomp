// CV_ChapelOfEvilScript::EndMission() [propagated label; the body is a Knothole Glade house step] -- script thread:
// wait for the go flag (+0x4b), then on every region-loaded message flag it (+0x52) and, while KnotholeGlade is
// loaded, make the ChiefsHut house scripted and override its automatic locking
// Leaf: EndMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE1770, 363 bytes (padded to 368).
// Callees: CCharString() @ 0x0099E4B0, CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          CScriptBase::IsActiveThreadTerminating @ 0x00CB7940, ~CScriptThing @ 0x004AA840; CGameScriptInterface
//          slots 7 NewScriptFrame (+0x1c), 12 IsRegionLoaded (+0x30), 22 MsgOnRegionLoaded (+0x58), 72
//          GetThingWithScriptName (+0x120), 431 OverrideAutomaticHouseLocking (+0x6bc), 433 SetBuyableHouseAsScripted
//          (+0x6c4).
// Layout: interface at +0x40; script bools +0x4b (go) and +0x52 (region seen), names unknown (propagated label).
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
    unsigned char _pad48[0x4b - 0x48];
    bool          Go;                                    // +0x4b
    unsigned char _pad4c[0x52 - 0x4c];
    bool          RegionSeen;                            // +0x52
    void EndMission();
};

void CV_ChapelOfEvilScript::EndMission()
{
    while (!Go) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CCharString region;
    while (!IsActiveThreadTerminating()) {
        if (GSI->MsgOnRegionLoaded(region)) {
            if (IsActiveThreadTerminating())
                return;
            RegionSeen = true;
            if (GSI->IsRegionLoaded("KnotholeGlade")) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->SetBuyableHouseAsScripted(GSI->GetThingWithScriptName("ChiefsHut"), true);
                GSI->OverrideAutomaticHouseLocking(GSI->GetThingWithScriptName("ChiefsHut"), true);
            }
        }
        GSI->NewScriptFrame();
    }
}