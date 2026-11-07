// NScript::CV_LostTraderScript::CLostTraderBanditSpawn::Main() -- lost-trader bandit spawner: spawn a random bandit at Me and wait for it to die; wait until Me is off camera and the hero over 15 units away, spawn a second bandit set on the hero and wait for it to die; wait for the parent's ReachedTrader flag and the same off-camera condition, spawn another bandit on the hero and wait for it to die; once the hero is within 30 units spawn a last bandit and remove Me
// Shape (chunk 835): shared headers with a row-local `inline CScriptThing::~CScriptThing() { }` (the inline-once dtor rule: the first bandit's terminate exit expands the dtor, every later exit calls 0x004AA840); `attacker` is a VALUE local and its two wait loops read it through a same-type `reinterpret_cast<const CScriptThing&>` (the chunk 820 cast lever, owner ruling 2026-11-07) so VC7.1 keeps IsAlive virtual and hoists `lea ecx,[esp+14h]` above the terminate jne; the first bandit's wait devirtualises the inline CScriptThing::IsAlive body (PImp.Data test + slot 75 on the Imp) as retail does.
// Leaf: ?Main@CLostTraderBanditSpawn@CV_LostTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA5230, 1117 bytes, thiscall, plain `ret`, no EH frame.
// Callees: CV_LostTraderScript::GetRandomBanditName 0x00EA5690, CScriptThing::operator= 0x008AB980 / ~CScriptThing 0x004AA840,
//   IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CCharString 0x0099EBF0 / ~ 0x0099EAE0;
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 70 GetHero, 91 CreateCreature, 108 RemoveThing, 421 IsCameraPosOnScreen,
//   451 GiveThingBestEnemyTarget; CScriptThing slots 6 GetPos, 75 IsAlive. Data: "" 0x0122D70E.
// Layout: engine/CV_LostTraderScript.h (opt-in FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BANDITNAME declares GetRandomBanditName): CEntityScriptBase
//   {vptr, Game +0x04, Me +0x08}, ParentClass& +0x14, PMasterData +0x18; parent ReachedTrader +0x4c (Ego_r.pdb member name).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BANDITNAME
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_LostTraderScript.h"
#include "engine/fable_script_distance.h"
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline CScriptThing::~CScriptThing() { }

void NScript::CV_LostTraderScript::CLostTraderBanditSpawn::Main()
{
    CScriptThing bandit = Game->CreateCreature(ParentClass.GetRandomBanditName(), Me.GetPos(), "", false);
    while (bandit.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (Game->IsCameraPosOnScreen(Me.GetPos()) || IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing attacker = Game->CreateCreature(ParentClass.GetRandomBanditName(), Me.GetPos(), "", false);
    Game->GiveThingBestEnemyTarget(attacker, Game->GetHero());
    while (reinterpret_cast<const CScriptThing&>(attacker).IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass.ReachedTrader) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (Game->IsCameraPosOnScreen(Me.GetPos()) || IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    attacker = Game->CreateCreature(ParentClass.GetRandomBanditName(), Me.GetPos(), "", false);
    Game->GiveThingBestEnemyTarget(attacker, Game->GetHero());
    while (reinterpret_cast<const CScriptThing&>(attacker).IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 30.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    attacker = Game->CreateCreature(ParentClass.GetRandomBanditName(), Me.GetPos(), "", false);
    Game->RemoveThing(Me, false, true);
}