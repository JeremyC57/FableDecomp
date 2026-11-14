// NScript::CQ_TraderConflictGoodScript::CBanditExtra::Main() -- an extra bandit of the trader-conflict (good) quest: pause half a second; while fewer than ten creatures are alive and the hero is more than 14 m away, spawn a level-2 bandit grunt at Me, roll a 1-in-(distance / 15, at most 3) chance to give it the hero as its best enemy target, and rebuild the live-enemy list
// Leaf: ?Main@CBanditExtra@CQ_TraderConflictGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DFCA90, 373 bytes (`ret`); entity vtable 0x012DEE14 slot 1.
// Identity: Ego_r.pdb ?Main@CBanditExtra@CQ_TraderConflictGoodScript@NScript@@EAEXXZ (0x00E3D170) + the landed Alloc 0x00DFBED0 (vtable
//   0x012DEE14, whose slot 1 is this function). Callees (reloc_bindings / CGameScriptInterfaceBase slots): Pause 376, GetHero 70,
//   CreateCreature 91 (hidden-result CScriptThing), GiveThingBestEnemyTarget 451; IsDistanceBetweenThingsOver 0x00CBE3EA,
//   GetDistanceBetweenThings 0x00CBE512, rand 0x00BFEB16, CQ_TraderConflictGoodScript::UpdateLiveEnemies 0x00DFC320.
// Retail-asm: GFFloatToLongCeil (engine/GFFloatToLongCeil.h) expanded at 0x00DFCB72..0x00DFCB9D `mov [ebp-0Ch],0.5f ; mov [ebp-4],1.0f ;
//   fstp [ebp-10h] ; fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-8] ; fild [ebp-8] ; fsub [ebp-4] ; fstp [ebp-14h] ;
//   mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; dec dword ptr [ebp-8]` (the ebp frame of the asm block).
// Strings: 0x012DEEA8 CREATURE_BANDIT_GRUNT_LEVEL2, 0x0122D70E "" (the script name). 1/15 = 0x0126B7DC.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_TRADERCONFLICTGOODSCRIPT_BANDITEXTRA
#define FABLE_RETAIL_SCRIPT_GETDISTANCEBETWEENTHINGS
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_TraderConflictGoodScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
#include "engine/GFFloatToLongCeil.h"

void NScript::CQ_TraderConflictGoodScript::CBanditExtra::Main()
{
    Game->Pause(0.5f);
    if (ParentClass.AllCreatures.size() < 10 && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 14.0f)) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing bandit = Game->CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL2", Me.GetPos(), "", false);
        long chance = GFFloatToLongCeil(GetDistanceBetweenThings(bandit, Game->GetHero()) * (1.0f / 15.0f));
        if (chance > 3) {
            if (IsActiveThreadTerminating()) return;
            chance = 3;
        }
        if (rand() % chance == 0) {
            if (IsActiveThreadTerminating()) return;
            Game->GiveThingBestEnemyTarget(bandit, Game->GetHero());
        }
        ParentClass.UpdateLiveEnemies();
    }
}
