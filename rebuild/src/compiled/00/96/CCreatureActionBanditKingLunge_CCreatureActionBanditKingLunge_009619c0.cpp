// CCreatureAction_BanditKingLunge::CCreatureAction_BanditKingLunge(CThingCreatureBase& creature, CThing& target) --
// base CCombatActionBase(creature, target, 150, zeroed CSoundPair)
// Leaf: ??0CCreatureAction_BanditKingLunge@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009619C0, oracle length 58 bytes, `ret 8`.
// Retail bytes:  sub esp,8 ; mov edx,[thing] ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[owner] ; push 0x96 ; push ecx ; push edx ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase ; mov dword ptr [esi],offset CCreatureAction_BanditKingLunge::`vftable' ; mov eax,esi ; pop esi ; add esp,8 ; ret 8
// Two-argument combat-action constructor: the base @ 0x00857F30 takes the creature, the target, the action priority 150 (0x96) and a
// two-dword sound pair built zeroed on the stack; then this class's vtable (0x01295F64, rtti-vtables.tsv).
// NF72 (round 7, q10860): moved off its local CSoundPair / CCombatActionBase / class models (pointer arguments) onto the shared
//   engine/CCreatureAction_BanditKingLunge.h (opt-in FABLE_RETAIL_CCREATUREACTION_BANDITKINGLUNGE, NF67),
//   engine/CCombatActionBase.h (TARGETCTOR: the Ego_r.pdb (creature, target, priority, sound) constructor 0x00857F30) and engine/CSoundPair.h.
//   Evidence: Ego_r.pdb + FableWin.pdb ??0CCreatureAction_BanditKingLunge@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z (the class's only
//   non-copy constructor); the ported caller CAIStateGroup_BanditKingLunge::Update 0x00961880 binds it.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGLUNGE
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#include "rebuild_abi.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_BanditKingLunge.h"

CCreatureAction_BanditKingLunge::CCreatureAction_BanditKingLunge(CThingCreatureBase& creature, CThing& target)
    : CCombatActionBase(creature, target, 150, CSoundPair())
{
}
