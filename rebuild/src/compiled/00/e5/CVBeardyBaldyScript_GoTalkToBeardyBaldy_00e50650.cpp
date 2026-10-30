// CV_BeardyBaldyScript::GoTalkToBeardyBaldy() -- put the quest marker on the minimap over BB_BeardyBaldyMan when he lives and take it off the barber nearest the hero when she lives
// Leaf: GoTalkToBeardyBaldy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E50650, 400 bytes (a byte-identical twin at 0x00E50E40 sits in another TU).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, ~CScriptThing @ 0x004AA840 / inline (counted release, operator delete @ 0x00BFE9BC, ~CBaseClass
//          @ 0x0099A2E0), CScriptThing::IsAlive inline (game-side thing slot 75, +0x12c); CGameScriptInterface slots
//          70 GetHero (+0x118), 72 GetThingWithScriptName (+0x120), 81 GetNearestWithDefName (+0x144), 350
//          MiniMapAddMarker (+0x578), 352 MiniMapRemoveMarker (+0x580).
// Layout: CScriptThing model as the landed CQ_HangingTreeGoodScript (0x00D74F80); the script's own members are unused.
// Shape: the two handles are destroyed out of line at the terminating returns and expanded inline at the end.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


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

struct CV_BeardyBaldyScript : CQuestScriptBase {
    void GoTalkToBeardyBaldy();
};

void CV_BeardyBaldyScript::GoTalkToBeardyBaldy()
{
    CScriptThing beardy = GSI->GetThingWithScriptName("BB_BeardyBaldyMan");
    CScriptThing barber = GSI->GetNearestWithDefName(GSI->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
    if (beardy.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        GSI->MiniMapAddMarker(beardy, "HUD_ORB_QUEST_VIGNETTE");
    }
    if (barber.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        GSI->MiniMapRemoveMarker(barber);
    }
}