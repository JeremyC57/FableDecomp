// 00e95770 NScript::CV_HauntedHouseScript::CM_UndeadHere::Main
// Retail: 0x00E95770, 1144 bytes, thiscall, complete body through plain ret.
// Flags: none
// Leaf: ?Main@CM_UndeadHere@CV_HauntedHouseScript@NScript@@EAEXXZ
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
// Original donor calls IsDead; its real native body calls virtual IsAlive.
inline bool CScriptThing::IsDead() const { return !IsAlive(); }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }

void NScript::CV_HauntedHouseScript::CM_UndeadHere::Main()
{
    TRetVoid _Fail_Val;
    while (!ParentClass.InHouse) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing MyCreature;
    while (!ParentClass.CellarDoorOpen) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.UndeadAlive < ParentClass.MaxUndead) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Counter < 10 && IsDistanceBetweenThingsOver(Me, Game.GetHero(), 3.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.UndeadAlive++;
                if (rand() % 2 == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.SetCreatureCreationDelayFrames(1);
                    MyCreature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD_HH", Me.GetPos(), "HHUndead", true);
                    Game.ResetCreatureCreationDelayFrames();
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.SetCreatureCreationDelayFrames(1);
                    MyCreature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_AXE_HH", Me.GetPos(), "HHUndead", true);
                    Game.ResetCreatureCreationDelayFrames();
                }
                Counter++;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.Pause(1.0f);
                if (ParentClass.UndeadAlive < ParentClass.MaxUndead && !ParentClass.CellarDoorOpen
                    && IsDistanceBetweenThingsOver(Me, Game.GetHero(), 3.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.UndeadAlive++;
                    if (rand() % 2 == 0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.SetCreatureCreationDelayFrames(1);
                        MyCreature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_SWORD_HH", Me.GetPos(), "HHUndead", true);
                        Game.ResetCreatureCreationDelayFrames();
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game.SetCreatureCreationDelayFrames(1);
                        MyCreature = Game.CreateCreature("CREATURE_OK_UNDEAD_SOLDIER_AXE_HH", Me.GetPos(), "HHUndead", true);
                        Game.ResetCreatureCreationDelayFrames();
                    }
                    Counter++;
                }
            }
            while (!MyCreature.IsDead()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}