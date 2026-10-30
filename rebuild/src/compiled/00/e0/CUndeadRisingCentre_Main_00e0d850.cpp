// CUndeadRisingCentre::Main() -- Undead Rising quest: until the hero comes within 3 units of the centre marker, every 10 seconds play the next of three "light" voice-over lines; then flag HeroReachedCentre on the quest, remove the minimap marker and idle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0D850, length 545 bytes.
// Callees: IsActiveThreadTerminating 0x00F35B30; IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 87..90 the timer calls (through the
//   global interface 0x0143E8F8, as CTimer does), 352 MiniMapRemoveMarker, 364 AddNewConversation, 365 AddPersonToConversation,
//   366 AddLineToConversation.
// Layout: Ego_r.pdb CUndeadRisingCentre (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, DoorOpen +0x1c. CQ_UndeadRisingScript is retail = Ego_r.pdb - 0x14: HeroReachedCentre +0x50.
// Shape: a CTimer local (RegisterTimer in the ctor, DeregisterTimer in the dtor); the line index is a frame word and the
//   three lines are a switch (case 0, case 1, default).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCountBlock { long References; };
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C {vptr, PImp_Data, PImp_Info}
    void*        PImpData;
    CCountBlock* PImpInfo;
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;                // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
struct CQ_UndeadRisingScript {
    unsigned char _pad[0x50];
    bool          HeroReachedCentre;                     // +0x50
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CUndeadRisingCentre : public CEntityScriptBase {
    void*                  VTable;
    CGameScriptInterface*             Game;                         // +0x04
    CScriptThing           Me;                           // +0x08
    CQ_UndeadRisingScript* ParentClass;                  // +0x14
    void*                  PMasterData;                  // +0x18
    bool                   DoorOpen;                     // +0x1c
    void Main();
};

void CUndeadRisingCentre::Main()
{
    CTimer timer;
    timer = 0;
    long line = 0;
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 3.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        if (timer.GetTime() <= 0) {
            if (IsActiveThreadTerminating()) return;
            switch (line) {
            case 0:
                Game->AddLineToConversation(conversation, "TEXT_QST_049_VOICE_LIGHT_FIRST", false, Me, Game->GetHero());
                break;
            case 1:
                Game->AddLineToConversation(conversation, "TEXT_QST_049_VOICE_LIGHT_SECOND", false, Me, Game->GetHero());
                break;
            default:
                Game->AddLineToConversation(conversation, "TEXT_QST_049_VOICE_LIGHT_THIRD", false, Me, Game->GetHero());
                break;
            }
            ++line;
            timer = 10;
        }
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass->HeroReachedCentre = true;
    Game->MiniMapRemoveMarker(Me);
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}