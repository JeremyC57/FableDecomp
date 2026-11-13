// NScript::CQ_MinionClifftopChaseScript::CMinionSpawnAdv::Main() -- the advanced minion spawner of the clifftop chase: wait until the hero is within 4 units and the quest's MinionCount is under the global MCC_MinionCounter; then for every MinionHere marker whose data string equals Me's (while the count is under the limit) play SUMMON_CREATURE_SUMMON_EVIL, pause 0.1 s and create a CREATURE_MINION_WARDOG named MinionEscort; repeat over the MinionSummonerHere markers with CREATURE_MINION_DREADWINGS; finally remove Me
// Grade: DIFFER(1546v1447) -- inline-threshold residue: the post-loop-1 thread check expands ~vector<CScriptThing> where retail sends it to the in-loop pad calling 0x008AC970 (only the post-loop-2 exit and the natural exit expand), then the trailing zero register (edi v ebp) differs (84 lines)
// Next idea: a spelling that leaks less body frequency out of loop 1 than the for-loop but more than while (b4 cools every later exit, 1302 bytes); diagnostic /d2inlT60 or T66 gives the right split (md79), so this is the function-wide inline budget, a park
// Leaf: ?Main@CMinionSpawnAdv@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA8BF0, 1447 bytes, thiscall, plain `ret` (three epilogue copies), frame 0x4c.
// Identity (proven): Ego_r.pdb + FableWin.pdb ?Main@CMinionSpawnAdv@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ; the single .rdata
//   reference is slot 1 of the CMinionSpawnAdv entity vtable 0x012D75F0 (slot 0 scalar dtor 0x00DA6150, slot 3 GetParentScript 0x00DA6140).
// Callees: IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CBasicString<char>::Compare 0x00411570 (CCharString::operator== inline), ~CScriptThing 0x004AA840, std::vector<CScriptThing>::erase
//   0x008AC9D0 (clear()) and ~vector 0x008AC970, free 0x00BFEA14 (STLport allocator); pGlobalScriptDef 0x0143E90C (MCC_MinionCounter +0xd58);
//   CScriptThing slots 3 GetDataString, 6 GetPos; CGameScriptInterfaceBase slots 7, 70 GetHero, 75 GetAllThingsWithScriptName,
//   91 CreateCreature, 100 CreateEffect, 108 RemoveThing, 376 Pause.
// Layout: engine/CQ_MinionClifftopChaseScript.h (opt-in FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_MINIONSPAWNADV): ParentClass +0x14
//   (MinionCount +0x68), PMasterData +0x18.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_MINIONSPAWNADV
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_MinionClifftopChaseScript.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_def_global.h"
#include "engine/C3DVector.h"

void NScript::CQ_MinionClifftopChaseScript::CMinionSpawnAdv::Main()
{
    while (!(IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 4.0f) && ParentClass.MinionCount < pGlobalScriptDef->MCC_MinionCounter)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    std::vector<CScriptThing> spawners;
    Game->GetAllThingsWithScriptName("MinionHere", spawners);
    for (unsigned long i = 0; i < spawners.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        if (spawners[i].GetDataString() == Me.GetDataString() && ParentClass.MinionCount < pGlobalScriptDef->MCC_MinionCounter) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateEffect("SUMMON_CREATURE_SUMMON_EVIL", spawners[i].GetPos(), "", 0.0f, false, false);
            Game->Pause(0.1f);
            Game->CreateCreature("CREATURE_MINION_WARDOG", spawners[i].GetPos(), "MinionEscort", true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    spawners.clear();
    Game->GetAllThingsWithScriptName("MinionSummonerHere", spawners);
    for (unsigned long j = 0; j < spawners.size(); ++j) {
        if (IsActiveThreadTerminating())
            return;
        if (spawners[j].GetDataString() == Me.GetDataString() && ParentClass.MinionCount < pGlobalScriptDef->MCC_MinionCounter) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateEffect("SUMMON_CREATURE_SUMMON_EVIL", spawners[j].GetPos(), "", 0.0f, false, false);
            Game->Pause(0.1f);
            Game->CreateCreature("CREATURE_MINION_DREADWINGS", spawners[j].GetPos(), "MinionEscort", true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}
