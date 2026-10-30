// CQ_BanditCampHoldingScriptScript::Main() -- once per save, wait for the BanditCampPath_1 level and record PathReached; then wait for the BanditCampPathEntrance region and open the OakValeBanditRaidGate door if it is shut
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D129C0, 416 bytes, plain `ret` (vtable slot at 0x012C9E1C).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("BanditCampPath_1" 0x012C85D8,
//   "BanditCampPathEntrance" 0x012C83D0, "OakValeBanditRaidGate" 0x012C9E28), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, ~CScriptThing 0x004AA840; CGameScriptInterface (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded
//   (+0x2c), 12 IsRegionLoaded (+0x30), 72 GetThingWithScriptName (+0x120), 425 OpenDoor (+0x6a4); the thing's
//   implementation slot 64 IsOpenDoor (+0x100) behind the inlined null-safe wrapper.
// Shape: WAIT_UNTIL(x) is `while (!x) { NewScriptFrame(); if (terminating) return; }` (neg/sbb/inc on the bool), as
//   the landed CV_SingingStonesScript::WatchForRegionLeaving 0x00ED2A50.
// Layout (retail = Ego_r.pdb - 0x14): PathReached +0x48.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline bool CScriptThing::IsOpenDoor() const { if (!PImp.Data) return false; return PImp.Data->IsOpenDoor(); }
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
namespace NScript {
class CQ_BanditCampHoldingScriptScript : public CQuestScriptBase {
public:
    bool PathReached;                                       // +0x48
    void Main();
};
} // namespace NScript
using NScript::CQ_BanditCampHoldingScriptScript;

void NScript::CQ_BanditCampHoldingScriptScript::Main()
{
    if (!PathReached) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("BanditCampPath_1")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        PathReached = true;
    }
    while (!Game->IsRegionLoaded("BanditCampPathEntrance")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing gate = Game->GetThingWithScriptName("OakValeBanditRaidGate");
    if (!gate.IsOpenDoor()) {
        if (IsActiveThreadTerminating())
            return;
        Game->OpenDoor(gate);
    }
}