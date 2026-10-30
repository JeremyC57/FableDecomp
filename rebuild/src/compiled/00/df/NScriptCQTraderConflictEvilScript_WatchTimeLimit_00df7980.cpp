// CQ_TraderConflictEvilScript::WatchTimeLimit() -- start the escort time limit once the quest data says so, show it as a quest-info timer until the mission succeeds, and flag the data if time remained
// Leaf: ?WatchTimeLimit@CQ_TraderConflictEvilScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF7980, length 412 bytes (padded to 416).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; CCharString(const char*, long) @ 0x0099EBF0 and
//   ~CCharString @ 0x0099EAE0; CTimer::~CTimer @ 0x00CD4470 (the one out-of-line destructor site); through the global
//   game interface 0x0143E8F8: RegisterTimer/DeregisterTimer/SetTimer/GetTimer (slots 87-90); through the script's own
//   interface (+0x40): NewScriptFrame (slot 7), DisplayQuestInfo (321), AddQuestInfoTimer (326),
//   RemoveQuestInfoElement (338).
// Identity: spawned (function pointer stored at 0x00DF6615) from the Evil script's Main; its twin 0x00DFAEB0 is the
//   Good script's copy (data flag +0x132 instead of +0x131).
// Layout (retail = Ego_r PDB -0x18 after the 12-byte retail AllCreatures vector): MissionSucceeded +0x68. The quest
//   data (+0x44) flags +0x129 / +0x131 and the script-def time limit (+0xF80 of the def at 0x0143E90C) are unnamed.
// Shape: a CTimer local whose inline destructor deregisters the timer at every return; the first wait loop also ends
//   when the 5-second timer runs out (then a terminate check whose two arms both just return). The "HUD_CLOCK_ICON"
//   literal is passed straight to the `const CCharString&` parameter: the implicit conversion temporary is built before
//   the interface pointer is loaded (an explicit CCharString(...) temporary interleaves with the pushes).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTimer;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }   // 0x00CD4470 where not inlined
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};
class CScriptDef { public:
    unsigned char _pad[0xf80];
    long TraderConflictTimeLimit;                       // +0xF80 (unnamed in the PDB view)
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_TraderConflictData {
    unsigned char _pad[0x129];
    bool TimeLimitBeaten;                               // +0x129
    unsigned char _pad12a[7];
    bool StartTimeLimit;                                // +0x131
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_TraderConflictEvilScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    CQ_TraderConflictData* Data;                        // +0x44
    unsigned char _pad48[0x20];                         // AllCreatures +0x48 .. NextTimeToSpawnGuards +0x64
    bool MissionSucceeded;                              // +0x68
    void WatchTimeLimit();
};
} // namespace NScript
using NScript::CQ_TraderConflictEvilScript;

void NScript::CQ_TraderConflictEvilScript::WatchTimeLimit()
{
    CTimer timer;
    timer.SetTime(5);
    while (!Data->StartTimeLimit) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            return;
        }
    }
    timer.SetTime(pGlobalScriptDef->TraderConflictTimeLimit);
    long element = Game->AddQuestInfoTimer(timer, "HUD_CLOCK_ICON", 1.0f);
    Game->DisplayQuestInfo(true);
    while (!MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(element);
    if (timer.GetTime() > 0) {
        if (IsActiveThreadTerminating())
            return;
        Data->TimeLimitBeaten = true;
    }
}