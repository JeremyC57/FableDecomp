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
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                  // +0x04
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
};
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
class CGameScriptInterface { public:                              // vtable 0x01260F0C (cgsi_slots_aligned.tsv)
    VPAD4(p0_0) VPAD2(p0_1) VPAD1(p0_2)                                          // slots 0..6
    virtual void NewScriptFrame() const;                                         // slot 7 (+0x1c)
    VPAD32(p8_0) VPAD16(p8_1) VPAD8(p8_2) VPAD4(p8_3) VPAD2(p8_4)                // slots 8..69
    virtual CScriptThing& GetHero();                                             // slot 70 (+0x118)
    VPAD4(p71_0) VPAD2(p71_1)                                                    // slots 71..76
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const; // slot 77 (+0x134)
    VPAD8(p78_0) VPAD1(p78_1)                                                    // slots 78..86
    virtual long RegisterTimer();                                                // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);                                    // slot 88 (+0x160)
    virtual void SetTimer(long timer, long time);                                // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;                                     // slot 90 (+0x168)
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& pos, const CCharString& scriptName, bool b) const; // slot 91 (+0x16c)
    VPAD16(p92_0)                                                                // slots 92..107
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    VPAD256(p109_0) VPAD32(p109_1) VPAD16(p109_2) VPAD8(p109_3)                  // slots 109..420
    virtual bool IsCameraPosOnScreen(const C3DVector& pos) const;                // slot 421 (+0x694)
    VPAD16(p422_0) VPAD8(p422_1) VPAD4(p422_2) VPAD1(p422_3)                     // slots 422..450
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const; // slot 451 (+0x70c)
    VPAD8(p452_0) VPAD4(p452_1) VPAD1(p452_2)                                    // slots 452..464
    virtual void EntityAttachToScript(const CScriptThing& thing, const CCharString& script) const; // slot 465 (+0x744)
    VPAD8(p466_0) VPAD2(p466_1) VPAD1(p466_2)                                    // slots 466..476
    virtual void SetThingPersistent(const CScriptThing& thing, bool persistent) const; // slot 477 (+0x774)
    VPAD128(p478_0) VPAD32(p478_1) VPAD16(p478_2) VPAD1(p478_3)                  // slots 478..654
    virtual CCharString GetActiveQuestName() const;                              // slot 655 (+0xa3c)
    VPAD32(p656_0) VPAD2(p656_1)                                                 // slots 656..689
    virtual unsigned long PlaySoundOnThing(const CScriptThing& thing, const CCharString& sound) const; // slot 690 (+0xac8)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                          // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
    __forceinline ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }   // out-of-line copy 0x00CD4470
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
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
namespace NScript {
class CIsThingAlive { public:                                   // vtable 0x012C32F0
    virtual void v00();
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
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
    timer.SetTime(2);
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
            timer.SetTime(2);
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