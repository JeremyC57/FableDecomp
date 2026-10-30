// CQ_HeroSoulsNostroScript::WatchForNostroHit() -- note Nostro's health, start an 80-second timer, and wait while Nostro has lost less than 100 health; if the timer runs out first, Nostro tells the hero the guildmaster message in a one-line conversation
// Leaf: WatchForNostroHit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D88260, 438 bytes (448 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("Nostro" 0x012D4E38,
//   "TEXT_QST_B04_GM_MSG_NOSTRO" 0x012D4E70), CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CScriptThing
//   0x004AA840; the global game script interface 0x0143E8F8 (the CTimer local): RegisterTimer (slot 87, +0x15c),
//   DeregisterTimer (88, +0x160), SetTimer (89, +0x164), GetTimer (90, +0x168); through the quest's interface (+0x40):
//   NewScriptFrame (7), GetHero (70, +0x118), GetThingWithScriptName (72, +0x120), GetHealth (264, +0x420),
//   AddNewConversation (364, +0x5b0), AddPersonToConversation (365, +0x5b4), AddLineToConversation (366, +0x5b8).
//   Float 100.0f 0x01230A08.
// Shape: the CTimer local as the landed CQ_DragonBossFightScript::JackTaunts 0x00D26A50 (index kept in ebx,
//   deregistered on every exit); the health threshold is a named float (frame word shared later with the line
//   temporary); the timer arm breaks to the trailing terminating test.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long seconds) { g_pGameScriptInterface->SetTimer(TimerIndex, seconds); return *this; }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HeroSoulsNostroScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    void WatchForNostroHit();
};
} // namespace NScript
using NScript::CQ_HeroSoulsNostroScript;

void NScript::CQ_HeroSoulsNostroScript::WatchForNostroHit()
{
    CScriptThing nostro = Game->GetThingWithScriptName("Nostro");
    float threshold = Game->GetHealth(nostro) - 100.0f;
    CTimer timer;
    timer = 80;
    while (Game->GetHealth(nostro) > threshold) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(nostro, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B04_GM_MSG_NOSTRO", false, nostro, Game->GetHero());
            break;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}