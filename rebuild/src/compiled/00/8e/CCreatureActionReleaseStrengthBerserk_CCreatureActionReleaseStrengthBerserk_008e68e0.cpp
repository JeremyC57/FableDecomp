// CCreatureAction_ReleaseStrengthBerserk::CCreatureAction_ReleaseStrengthBerserk(CThingCreatureBase&) -- base CCombatActionBase(owner, 145, zeroed CSoundPair), then animation "STRENGTH_CAST_BERSERK"
// Leaf: ??0CCreatureAction_ReleaseStrengthBerserk@@QAE@AAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E68E0, oracle length 90 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,8 ; xor eax,eax ; push esi ; mov [esp+4],eax ; mov [esp+8],eax ; lea eax,[esp+4] ; push eax ; mov esi,ecx ; mov ecx,[esp+14h] ; push 0x91 ; push ecx ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase ; push -1 ; push offset lit ; lea ecx,[esp+18h] ; mov dword ptr [esi],offset vftable ; call CCharString::CCharString ; lea edx,[esp+10h] ; push edx ; mov ecx,esi ; call SetAnimationName ; lea ecx,[esp+10h] ; call CCharString::~CCharString ; mov eax,esi ; pop esi ; add esp,8 ; ret 4
// Combat-action constructor: the base @ 0x00858030 takes the owner, the action code 145 (0x91)
// and a two-dword sound pair built zeroed on the stack by its inline constructor (register-zeroed
// stores: an embedded object's inline ctor), then this class's vtable (0x012874EC,
// rtti-vtables.tsv) and the animation name set through a scoped CCharString (ctor @ 0x0099EBF0, dtor @ 0x0099EAE0; the setter's manifest label is propagated, named as in family 20).
// One of 35 bodies of this shape.
// Identity: Ego_r.pdb / Ego_d.pdb ??0CCreatureAction_ReleaseStrengthBerserk@@QAE@AAVCThingCreatureBase@@@Z (the creature by
// reference; b12 landing re-port onto engine/CCreatureAction_ReleaseStrengthBerserk.h, was a local model taking a pointer).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#define FABLE_RETAIL_CCREATUREACTION_RELEASESTRENGTHBERSERK
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ReleaseStrengthBerserk.h"

CCreatureAction_ReleaseStrengthBerserk::CCreatureAction_ReleaseStrengthBerserk(CThingCreatureBase& creature)
    : CCombatActionBase(creature, 145, CSoundPair())
{
    {
        CCharString name("STRENGTH_CAST_BERSERK", -1);       // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetCombatAnimation(name);            // 0x008574D0
    }
}
