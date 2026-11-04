// NScript::CQ_UndeadRisingScript::CUndeadRisingFirefly::Main() -- Undead Rising firefly: once the quest reaches state 1, raise an undead soldier at a random grave, wait for it to die, then while the quest stays in state 1 keep raising a new one at a fresh random grave seven seconds after the last one dies
// Grade: DIFFER(1276v1223) -- 11 grades, best masked dl=32: retail calls ~CScriptThing (0x004AA840) out of line for the CreateCreature temporary in the loop and keeps one shared frame slot for both CreateCreature temporaries; this source (break-guarded loop body, __forceinline CTimer::operator=) expands that destructor. Next idea: a loop/guard shape that lifts the loop's operator= above the inline threshold but not the temporary's destructor (if-guarded body + plain inline gets both out of line, a2 dl=85); try the Aeon LUAGameflow port of this script for the original statement order.
// Leaf: ?Main@CUndeadRisingFirefly@CQ_UndeadRisingScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0CA50, 1223 bytes (int3 padded to 1232), plain `ret`.
// Identity: the CUndeadRisingFirefly entity class of CQ_UndeadRisingScript (landed Alloc 0x00E0C970 / GetParentScript 0x00E0CA10 /
//   Init 0x00E0CA20 / ??_G 0x00E0CA30 sit directly before it; Ego_r.pdb / FableWin.pdb
//   ?Main@CUndeadRisingFirefly@CQ_UndeadRisingScript@NScript@@EAEXXZ, the entity-script Main slot).
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CBaseClass() 0x0099A2D0 /
//   ~ 0x0099A2E0, ~CScriptThing 0x004AA840 (and inline), CScriptThing::operator= 0x008AB980 (and inline), operator delete 0x00BFE9BC,
//   ~CTimer 0x00CD4470 (and inline); Game slots 7 NewScriptFrame, 74 GetRandomThingWithScriptName, 91 CreateCreature,
//   512 EntitySetCutsceneBehaviour; pDSTGame 0x0143E8F8 slots 87 RegisterTimer, 88 DeregisterTimer, 89 SetTimer, 90 GetTimer;
//   the implementation's slots 6 GetPos / 75 IsAlive behind the inline null-safe wrappers.
//   Strings "UndeadRisingGrave", "UndeadRisingZombieAttacker", "CREATURE_OK_UNDEAD_SOLDIER_SWORD".
// Layout: as the landed same-TU CUndeadRisingBoss::Main 0x00E0D210 (CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14;
//   CQ_UndeadRisingScript quest_status +0x5c).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
class C3DVector { public: float X, Y, Z; };
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
enum ECutsceneBehaviour { CUTSCENE_BEHAVIOUR_2 = 2 };
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline bool CTimer::operator>(long time) const { return pDSTGame->GetTimer(TimerIndex) > time; }
namespace NScript {
class CQ_UndeadRisingScript {
public:
    unsigned char _pad00[0x5c];
    long          quest_status;                             // +0x5c
public:
    class CUndeadRisingFirefly;
};
}
using NScript::CQ_UndeadRisingScript;
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

namespace NScript {
class CQ_UndeadRisingScript::CUndeadRisingFirefly : public CEntityScriptBase {
public:
    CGameScriptInterface*  Game;                            // +0x04
    CScriptThing           Me;                              // +0x08
    CQ_UndeadRisingScript* ParentClass;                     // +0x14
    void*                  PMasterData;                     // +0x18
private:
    virtual void Main();
};
}

void NScript::CQ_UndeadRisingScript::CUndeadRisingFirefly::Main()
{
    while (ParentClass->quest_status != 1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing grave = Game->GetRandomThingWithScriptName("UndeadRisingGrave");
    CScriptThing zombie;
    CTimer timer;
    timer = 0;
    if (ParentClass->quest_status == 1) {
        if (IsActiveThreadTerminating())
            return;
        zombie = Game->CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD", grave.GetPos(), "UndeadRisingZombieAttacker", false);
        Game->EntitySetCutsceneBehaviour(zombie, (ECutsceneBehaviour)2);
        while (zombie.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        timer = 7;
    }
    while (ParentClass->quest_status == 1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        grave = Game->GetRandomThingWithScriptName("UndeadRisingGrave");
        while (timer > 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->quest_status != 1)
            break;
        if (IsActiveThreadTerminating())
            return;
        zombie = Game->CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD", grave.GetPos(), "UndeadRisingZombieAttacker", false);
        Game->EntitySetCutsceneBehaviour(zombie, (ECutsceneBehaviour)2);
        while (zombie.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        timer = 7;
    }
    if (IsActiveThreadTerminating())
        return;
}
