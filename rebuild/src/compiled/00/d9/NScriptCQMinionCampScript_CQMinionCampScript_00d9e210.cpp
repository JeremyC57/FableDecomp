// CQ_MinionCampScript::CQ_MinionCampScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012D6C60, ChantTimer and MinionSpawnTimer, the two hero-attacker script things, the empty Left/RightSpawnPoints lists and SpawnPointToSpawn
// Leaf: ??0CQ_MinionCampScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9E210, 197 bytes (padded to 208), `ret 8`.
// Identity: the manifest label "CV_FisticuffsClubScript" is propagated and wrong. The factory 0x00DA2F40 (`push 0ACh`,
//   this ctor) is registered at 0x00CD6D27 under "S_QMC"; size 0xac = Ego_r.pdb CQ_MinionCampScript 208 - 0x14 (script
//   base) - 2*8 (the two std::list members are a single 4-byte sentinel pointer in retail, 12 in the PDB).
// Callees: CScriptBase::CScriptBase 0x00CB8110, CGameScriptInterface slot 87 RegisterTimer (+0x15c, global interface
//   0x0143E8F8), CBaseClass::CBaseClass 0x0099A2D0 (CScriptThing base, vtable 0x01238C8C), CRT malloc 0x00BFEA0E (the
//   lists' 0x14-byte sentinel nodes: next, prev, CScriptThing).
// Shape: argument words before the derived vtable = inlined `__declspec(novtable)` intermediate base; each VC6-era list
//   ctor is `head = 0; node = malloc(0x14); node->next = node->prev = node; head = node;` (the shared zero register
//   stores the null head before the call; both mallocs' arguments are popped by one `add esp,8`).
// Layout (retail = PDB - 0x14): flags +0x48..+0x4d, NumberOfMinions +0x50, MaxNumberOfMinions +0x54, Min/Max spawn
//   periods +0x58/+0x5c, ChantTimer +0x60, ChantTimerBar +0x64, ChantTimerPaused +0x68, PausedTime +0x6c,
//   MinionSpawnTimer +0x70, CurrentStage +0x74, HeroAttackerOne +0x78, HeroAttackerTwo +0x84, LeftSpawnPoints +0x90,
//   RightSpawnPoints +0x94, ShouldSpawn +0x98, SpawnPointToSpawn +0x9c, BriarState +0xa8.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() : CBaseClass(), PImp() { }
struct CRetailList_CScriptThing {                  // retail std::list<CScriptThing>: one sentinel-node pointer
    struct node { node* next; node* prev; CScriptThing value; };
    node* head;
    CRetailList_CScriptThing() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};
namespace NScript {
class CQ_MinionCampScript : public CQuestScriptObject {
public:
    bool QuestStartScreenShown, IntroDone, BriarRoseAtAltar, MinionsSummoned, PortalSealed, MissionFailed;   // +0x48
    long NumberOfMinions;                          // +0x50
    long MaxNumberOfMinions;                       // +0x54
    float MinMinionSpawnPeriod;                    // +0x58
    float MaxMinionSpawnPeriod;                    // +0x5c
    CTimer ChantTimer;                             // +0x60
    long ChantTimerBar;                            // +0x64
    bool ChantTimerPaused;                         // +0x68
    long PausedTime;                               // +0x6c
    CTimer MinionSpawnTimer;                       // +0x70
    long CurrentStage;                             // +0x74 (MissionStage)
    CScriptThing HeroAttackerOne;                  // +0x78
    CScriptThing HeroAttackerTwo;                  // +0x84
    CRetailList_CScriptThing LeftSpawnPoints;      // +0x90
    CRetailList_CScriptThing RightSpawnPoints;     // +0x94
    bool ShouldSpawn;                              // +0x98
    CScriptThing SpawnPointToSpawn;                // +0x9c
    long BriarState;                               // +0xa8 (BriarRoseState)
    virtual ~CQ_MinionCampScript();
    CQ_MinionCampScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_MinionCampScript;
NScript::CQ_MinionCampScript::CQ_MinionCampScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}