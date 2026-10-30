// CDarkwoodAssassinSpawn::Main() -- Darkwood assassin ambush trigger: once the hero has stayed within the spawn distance with Me off screen for two timer ticks, spawn a DarkwoodAssassin at Me, make it persistent and attach it to the active quest, target the hero (1 in 5) or the nearest DarkwoodTrader, play a random shout at 18/12/8 units, then remove the trigger
// Leaf: ?Main@CDarkwoodAssassinSpawn@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E02E50, 1349 bytes, plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (CIsThingAlive filter, vtable 0x012C32F0, inline dtor),
//   CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CTimer dtor 0x00CD4470 (inline at the
//   early exits, out of line at the closing brace), CDefString::operator CCharString 0x00415D70, CCharString copy 0x0099EC30 /
//   (const char*) 0x0099EBF0 / dtor 0x0099EAE0, rand 0x00BFEB16, IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA
//   (__fastcall); GameCalls slots 7, 70, 77, 87-90 (timers on the global interface 0x0143E8F8), 91, 108, 421, 451, 465, 477,
//   655, 690. Literals "DarkwoodAssassin" 0x012DF5B0, "DarkwoodTrader" 0x012DF414.
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8} (landed CBanditCronies::Main 0x00E03810, same TU family).
//   pGlobalScriptDef (0x0143E90C): an int spawn distance +0xe14, the assassin def name (CDefString) +0xe18, the shout sound
//   vector<CDefString> +0x3ac/+0x3b0 (the bandit-crony shouts); member names are descriptive.
// Shape: the timer and shout are locals that live to the end; the timer dtor is __forceinline so the early exits (up to
//   the target pick) destroy it inline, while the exits in the 18/12/8 stages and the closing brace, under inline_depth(0),
//   call the out-of-line copy 0x00CD4470. The first wait loop is a for(;;) whose IsActiveThreadTerminating check sits at
//   the TOP (a bottom-of-loop check plus a guard before the loop swaps the vtable registers in the target pick).
extern "C" int __cdecl rand();                           // 0x00BFEB16
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
class CDefString { public:
    void* PStringData;
    operator CCharString() const;                        // 0x00415D70
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
__forceinline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
class CScriptDef { public:
    unsigned char _pad[0x3ac];
    CDefString* ShoutSoundsBegin;                        // +0x3ac
    CDefString* ShoutSoundsEnd;                          // +0x3b0
    CDefString* ShoutSoundsEndOfStorage;                 // +0x3b4
    unsigned char _pad3b8[0xe14 - 0x3b8];
    long        DarkwoodAssassinSpawnDistance;           // +0xe14
    CDefString  DarkwoodAssassinDefName;                 // +0xe18
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
using NScript::CIsThingAlive;
struct CDarkwoodAssassinSpawn {
    virtual ~CDarkwoodAssassinSpawn();
    virtual void Main();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    void SetPredicate(CIsThingAlive& filter);            // 0x00F35B10
    bool IsActiveThreadTerminating();                    // 0x00F35B30
};

void CDarkwoodAssassinSpawn::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    timer = 2;
    long spawnDistance = pGlobalScriptDef->DarkwoodAssassinSpawnDistance;
    unsigned long count = pGlobalScriptDef->ShoutSoundsEnd - pGlobalScriptDef->ShoutSoundsBegin;
    unsigned long index = rand() % count;
    CCharString shout = pGlobalScriptDef->ShoutSoundsBegin[index];
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)spawnDistance) && !Game->IsCameraPosOnScreen(Me.GetPos())) {
            if (IsActiveThreadTerminating())
                return;
        } else {
            if (IsActiveThreadTerminating())
                return;
            timer = 2;
        }
        if (timer.GetTime() == 0)
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing assassin = Game->CreateCreature(pGlobalScriptDef->DarkwoodAssassinDefName, Me.GetPos(), "DarkwoodAssassin", true);
    Game->SetThingPersistent(assassin, true);
    Game->EntityAttachToScript(assassin, Game->GetActiveQuestName());
    if (rand() % 5 == 0) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(assassin, Game->GetHero());
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(assassin, Game->GetNearestWithScriptName(assassin, "DarkwoodTrader"));
    }
    if (IsDistanceBetweenThingsOver(assassin, Game->GetHero(), 18.0f)) {
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        while (!IsDistanceBetweenThingsUnder(assassin, Game->GetHero(), 18.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                return;
#pragma inline_depth()
        }
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        Game->PlaySoundOnThing(assassin, shout);
    }
    if (IsDistanceBetweenThingsOver(assassin, Game->GetHero(), 12.0f)) {
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        while (!IsDistanceBetweenThingsUnder(assassin, Game->GetHero(), 12.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                return;
#pragma inline_depth()
        }
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        Game->PlaySoundOnThing(assassin, shout);
    }
    if (IsDistanceBetweenThingsOver(assassin, Game->GetHero(), 8.0f)) {
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        while (!IsDistanceBetweenThingsUnder(assassin, Game->GetHero(), 8.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
#pragma inline_depth(0)
                return;
#pragma inline_depth()
        }
        if (IsActiveThreadTerminating())
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        Game->PlaySoundOnThing(assassin, shout);
    }
    Game->RemoveThing(Me, false, true);
#pragma inline_depth(0)
}
#pragma inline_depth()