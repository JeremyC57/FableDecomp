// NScript::CQ_UndeadRisingScript::CUndeadRisingCentre::Main() -- Undead Rising quest: until the hero comes within 3 units of the centre marker, every 10 seconds play the next of three "light" voice-over lines; then flag HeroReachedCentre on the quest, remove the minimap marker and idle
// Leaf: ?Main@CUndeadRisingCentre@CQ_UndeadRisingScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members); the local's flattened PImpData / PImpInfo reads are the header's by-value PImp (CCountedPointer<CScriptThing>) .Data / .Info (+0x04 / +0x08).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0D850, length 545 bytes.
// Callees: IsActiveThreadTerminating 0x00F35B30; IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall); CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 87..90 the timer calls (through the
//   global interface 0x0143E8F8, as CTimer does), 352 MiniMapRemoveMarker, 364 AddNewConversation, 365 AddPersonToConversation,
//   366 AddLineToConversation.
// Layout: Ego_r.pdb CUndeadRisingCentre (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, DoorOpen +0x1c. CQ_UndeadRisingScript is retail = Ego_r.pdb - 0x14: HeroReachedCentre +0x50.
// Shape: a CTimer local (RegisterTimer in the ctor, DeregisterTimer in the dtor); the line index is a frame word and the
//   three lines are a switch (case 0, case 1, default).
// Naming wave 35: identity reg 00e0b36e "UndeadRisingCentre" in 00e0b160; Alloc 00e0d760; vtable 012e000c slot 1; PDB ?Main@CUndeadRisingCentre@CQ_UndeadRisingScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CUndeadRisingCentre::Main (wave3/labels_alloc.tsv); PDB ?Main@CUndeadRisingCentre@CQ_UndeadRisingScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CUndeadRisingCentre@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); outer stand-in CQ_UndeadRisingScript moved into NScript); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCountBlock { long References; };
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
namespace NScript {
class CQ_UndeadRisingScript {
public:
    unsigned char _pad[0x50];
    bool          HeroReachedCentre;                     // +0x50
public:
    class CUndeadRisingCentre;       // the entity class below (naming wave 35)
};
}
using NScript::CQ_UndeadRisingScript;
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_UndeadRisingScript::CUndeadRisingCentre : public CEntityScriptBase {
public:
        CGameScriptInterface*             Game;                         // +0x04
    CScriptThing           Me;                           // +0x08
    CQ_UndeadRisingScript* ParentClass;                  // +0x14
    void*                  PMasterData;                  // +0x18
    bool                   DoorOpen;                     // +0x1c
private:
    virtual void Main();                          // PDB ?Main@CUndeadRisingCentre@CQ_UndeadRisingScript@NScript@@EAEXXZ
};
}

void NScript::CQ_UndeadRisingScript::CUndeadRisingCentre::Main()
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