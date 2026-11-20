// CCreatureAction_BlockBroken::`scalar deleting destructor' @ 0x00739CC0 -- the implicit destructor (a direct call to
// ~CCombatActionBase 0x0062E900, no own vtable store), then free the object when bit 0 of the flags is set.
// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
// Leaf: ?ScalarDeletingDestructor@CCreatureAction_BlockBroken@@QAEPAXI@Z
// Naming (NF191, 2026-11-20, q4411 / q6405): the base destructor call binds the PDB ??1CCombatActionBase@@UAE@XZ (Ego_r.pdb
//   0x00545CE0 + FableWin.pdb 0x01D8EF00, virtual) through the shared engine/CCombatActionBase.h; was the stand-in member
//   ?Dtor@CCreatureAction_BlockBroken@@QAEXXZ. CCreatureAction_BlockBroken has no shared header (engine/_quarantine only, which is
//   not a shared header for the local-model rule): the row keeps its local class token and now sits on the shared base
//   CCombatActionBase (ghidra_out/rtti_bases.tsv: .?AVCCreatureAction_BlockBroken@@ direct base CCombatActionBase@0). The member
//   name ScalarDeletingDestructor stays the landed stand-in for the compiler-emitted ??_G (not renamed here).
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
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"

void __cdecl operator delete(void* p);

class CCreatureAction_BlockBroken : public CCombatActionBase { public: void* ScalarDeletingDestructor(unsigned flags); };

void* CCreatureAction_BlockBroken::ScalarDeletingDestructor(unsigned flags) {
    this->CCombatActionBase::~CCombatActionBase();
    if (flags & 1) ::operator delete(this);
    return this;
}
