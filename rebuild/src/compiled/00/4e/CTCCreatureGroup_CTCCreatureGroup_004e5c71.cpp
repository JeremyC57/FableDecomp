// CTCCreatureGroup::~CTCCreatureGroup() -- the implicit destructor: Members (+0x18, map<unsigned __int64, CGroupMemberData>) destroyed by
//   the out-of-line tree destructor, then ~CTCBase by tail call; no vptr store of its own
// Leaf: ??1CTCCreatureGroup@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5C71, 19 bytes, thiscall: push esi ; mov esi,ecx ; lea ecx,[esi+18h] ; call 0x004E350F ;
//   mov ecx,esi ; pop esi ; jmp 0x00686830. Called by ??_GCTCCreatureGroup 0x004E75BD (vftable 0x0124326C slot 0).
// Name: FableWin.pdb ??1CTCCreatureGroup@@UAE@XZ (public, and its module stream defines it in debug\tc_register.obj); Ego_d.pdb
//   (FableLib\debug\tc_register.obj) has CTCCreatureGroup::~CTCCreatureGroup; the binding table already maps the spelling here
//   (witness ~CTCCreatureGroupBoastingCrowd 0x00784BD0). The Ego_r and FableWin module streams of tc_creature_group.obj (every
//   out-of-line member) hold no destructor; Ego_r's tc_register.obj holds the inline ctor 0x004E5C2F. Callees: ~_Rb_tree of Members
//   0x004E350F (landed nf398_b1), ~CTCBase 0x00686830 (table line). NF405: was the free stand-in @FableDestVal_004e5c71@4 on a
//   row-local model (?inner / ?outer).
// Compiler-generated (NF405 measurements, evidence/): with `#pragma optimize("s", on)` a user-declared empty body (out of line or
//   inline) stores ??_7CTCCreatureGroup first (DIFFER 25v19); __declspec(novtable) drops that store but also the constructor's, which retail
//   0x004E5C2F keeps (`c7 06 6c 32 24 01`); ~CTCBase 0x00686830 stores its own vptr and slot 0 of both vftables is a ??_G, so the base
//   destructor is virtual and not empty. Only the implicit destructor gives retail's bytes (RELOCATION_MATCH), as for CMessageEvent 0x00687210.
// Size-optimised TU, as the tc_register.obj rows 0x004E5C2F / 0x004E5C61 / 0x004E75A0 / 0x004E75BD; /O2 inlines the whole tree destructor.
// clear and _M_erase are declared only (the define set of 0x004E350F): the tree destructor's copy equals that row's.
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CTCCREATUREGROUP_IMPLICIT_DTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY
#include "engine/CThingAICreature.h"
#include "engine/CGroupMemberData.h"
#include "engine/CTCCreatureGroup.h"

namespace {
// Fidelity: scaffolding, not retail -- the qualified explicit destructor call makes VC7.1 emit the implicit ~CTCCreatureGroup COMDAT.
void EmitCreatureGroupDestructor(CTCCreatureGroup* group)
{
    group->CTCCreatureGroup::~CTCCreatureGroup();
}
}
