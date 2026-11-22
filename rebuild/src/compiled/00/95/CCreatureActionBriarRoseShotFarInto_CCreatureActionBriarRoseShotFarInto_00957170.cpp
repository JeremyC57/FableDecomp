// CCreatureAction_BriarRoseShotFarInto::CCreatureAction_BriarRoseShotFarInto(CThingCreatureBase& creature, CThingCreatureBase& enemy) --
// base CCombatActionBase(creature, enemy, 250, GetSoundPair(creature, "SWORD_SWING")), then the combat animation "STRIKE_SHOT_FAR_INTO"
// Leaf: ??0CCreatureAction_BriarRoseShotFarInto@@QAE@AAVCThingCreatureBase@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957170, oracle length 124 bytes.
// Retail bytes:  sub esp,0Ch ; push esi ; push edi ; push -1 ; mov esi,ecx ; push offset "SWORD_SWING" ; lea ecx,[esp+10h] ; call CCharString::CCharString ; mov edi,[owner] ; lea eax,[esp+8] ; push e...
// The sound pair comes from the __fastcall GetSoundPair 0x008561A0: the name literal converts implicitly to a CCharString temporary,
// the 8-byte CSoundPair result lands in a frame temporary through the hidden pointer and is handed by reference to the base
// constructor 0x00857F30 with the action code 250 (0xfa); the string temporary dies after the base constructor, then this class's
// vtable (0x01294A2C, rtti-vtables.tsv) is stored and a scoped CCharString is passed to SetCombatAnimation 0x008574D0.
// Naming (NF191, 2026-11-20, q11777): Ego_r.pdb 0x0097BE30 + FableWin.pdb 0x02C02D20
//   ??0CCreatureAction_BriarRoseShotFarInto@@QAE@AAVCThingCreatureBase@@0@Z (two CThingCreatureBase references; the only landed binder
//   0x00956FE0 already binds it). Ported off the row-local models (CCharString, CSoundPair, CCombatActionBase, the class) onto the shared
//   headers (engine/CCreatureAction_BriarRoseShotFarInto.h _CLASS / _CTOR, engine/CCombatActionBase.h MELEE + TARGETCTOR + INITIALISE).
//   The callees now bind the PDB spellings: ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@AAVCThing@@JABVCSoundPair@@@Z (0x00857F30,
//   the q11677 reference form; was the pointer form) and ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z (0x008574D0;
//   was the stand-in ?SetName@CCombatActionBase@@QAEXABVCCharString@@@Z).
#define FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS  // NF264 (q12750): CCombatActionBase::AllowsCombinationAnims override, so emitted tables bind slot 66 = 0x0062EA10 as retail
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFARINTO_CLASS
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSESHOTFARINTO_CTOR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CSoundMap.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_BriarRoseShotFarInto.h"

CCreatureAction_BriarRoseShotFarInto::CCreatureAction_BriarRoseShotFarInto(CThingCreatureBase& creature, CThingCreatureBase& enemy)
    : CCombatActionBase(creature, enemy, 250, GetSoundPair(creature, "SWORD_SWING"))
{
    {
        CCharString name("STRIKE_SHOT_FAR_INTO", -1);      // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetCombatAnimation(name);                          // 0x008574D0
    }
}
