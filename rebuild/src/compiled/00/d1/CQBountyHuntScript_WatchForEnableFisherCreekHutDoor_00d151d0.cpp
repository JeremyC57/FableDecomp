// CQ_BountyHuntScript::WatchForEnableFisherCreekHutDoor() -- watch thread: wait for the Fisherman region to load, then open the fisherman's hut door if it is still there
// Leaf: WatchForEnableFisherCreekHutDoor
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D151D0, 531 bytes (padded to 544), plain `ret`.
// Callees: CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CBaseClass()
//   0x0099A2D0 (the local CScriptThing, vtable 0x01238C8C), IsActiveThreadTerminating 0x00CB7940, Compare 0x00411570
//   (the inlined CCharString == const char*), CCountedPointer::operator= 0x00704580 (the inlined CScriptThing
//   assignment), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840; through the interface
//   (+0x40): NewScriptFrame (slot 7), MsgOnRegionLoaded (slot 22, +0x58), GetThingWithScriptName (slot 72, +0x120),
//   OpenDoor (slot 425, +0x6a4); the door's implementation slot 75 IsAlive (+0x12c).
//   Strings "Fisherman" 0x012C9ED0, "FishermanHutDoor" 0x012CA4E0, "" 0x0122D70E.
// Shape: the landed CV_ChapelOfEvilScript::WatchForRegionEntry 0x00E6B750 region loop; the by-value door local's inline
//   destructor is expanded at the three in-loop terminate returns and called out of line (0x004AA840) after the loop.
#include <string.h>
#pragma intrinsic(strcmp)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() { }
__forceinline CScriptThing::~CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CQ_BountyHuntScript : CQuestScriptBase {
    void WatchForEnableFisherCreekHutDoor();
};

void CQ_BountyHuntScript::WatchForEnableFisherCreekHutDoor()
{
    CCharString region;
    CScriptThing door;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (region == "Fisherman")
            break;
        Game->NewScriptFrame();
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    door.PImp = Game->GetThingWithScriptName("FishermanHutDoor").PImp;
#pragma inline_depth()
    if (door.IsAlive()) {
#pragma inline_depth(0)
        if (IsActiveThreadTerminating())
            return;
#pragma inline_depth()
        Game->OpenDoor(door);
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
}