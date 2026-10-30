// CQ_WizardBattleScript::WatchForLeaving() -- wait until the hero is killed or the mission has ended; if the thread is still running and the mission was not won, fail it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1C4D0, length 164 bytes. A script thread started from Main (0x00E1A990)
// and from 0x00E1C0F0.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0, the game script interface (+0x40) GetHero (slot 70) and NewScriptFrame (slot 7), and the hero's
// CScriptThing::MsgIsKilledBy (virtual slot 18, +0x48).
// Layout (Ego_r.pdb - 0x14): MissionSucceeded +0x4b, MissionFailed +0x4c.
// Shape: the "" argument is an implicit CCharString temporary inside the `&&` loop condition, so VC keeps a
// construction-flag word in ebx (cleared at entry together with the temporary's frame word), materialises the condition
// in a frame byte and destroys the temporary conditionally; the loop stays top-tested.
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

#include "engine/CCountedPointer.h"


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
class CQ_WizardBattleScript : public CQuestScriptBase {
public:
    bool VillagersTurnOnHero;                            // +0x48
    bool VillagersChased;                                // +0x49
    bool FightersStart;                                  // +0x4a
    bool MissionSucceeded;                               // +0x4b
    bool MissionFailed;                                  // +0x4c
    bool MissionOver;                                    // +0x4d
    void WatchForLeaving();
};
} // namespace NScript
using NScript::CQ_WizardBattleScript;

void NScript::CQ_WizardBattleScript::WatchForLeaving()
{
    while (!GSI->GetHero().MsgIsKilledBy("") && !MissionFailed && !MissionSucceeded) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}