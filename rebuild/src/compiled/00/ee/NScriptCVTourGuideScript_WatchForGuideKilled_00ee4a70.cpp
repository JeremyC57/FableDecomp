// CV_TourGuideScript::WatchForGuideKilled() -- look up the TourGuideGuide, wait until he is alive (spawned), then wait until he has been killed (by anyone) and (unless the thread is terminating) record TourGuideKilled
// Leaf: ?WatchForGuideKilled@CV_TourGuideScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE4A70, 313 bytes (320 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC (MSVCR71), ~CScriptThing 0x004AA840 (the out-of-line copy), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; through the interface (+0x40) GetThingWithScriptName (slot 72, +0x120, CScriptThing by hidden pointer)
//   and NewScriptFrame (slot 7, +0x1c); the script thing's implementation slots 75 IsAlive (+0x12c) and 18
//   MsgIsKilledBy(const CCharString&) (+0x48) behind the inlined null-safe CScriptThing wrappers.
// Strings: "TourGuideGuide" 0x012F510C, "" 0x0122D70E.
// Shape: the landed CV_BeardyBaldyScript::WatchForBeardyBaldyDeath (0x00E503D0) pattern with two wait loops: both
//   strings are IMPLICIT conversions; the handle has four destructor sites (the terminating return inside the first
//   loop, the macro test after it, the return inside the second loop and the end) -- VC7.1 expands the inline
//   destructor at the first only and calls the out-of-line ~CScriptThing at the others (the last two tail-merge).
// Layout: TourGuideKilled +0x120 (landed WatchForClosingTime: TourFinished +0x123, PDB - 0x14).
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
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
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
class CV_TourGuideScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x120 - 0x48];                     // WaypointInfo +0x48 ..
    bool          TourGuideKilled;                          // +0x120
    void WatchForGuideKilled();
};
} // namespace NScript
using NScript::CV_TourGuideScript;
void NScript::CV_TourGuideScript::WatchForGuideKilled()
{
    CScriptThing guide = Game->GetThingWithScriptName("TourGuideGuide");
    while (!guide.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!guide.MsgIsKilledBy("")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    TourGuideKilled = true;
}