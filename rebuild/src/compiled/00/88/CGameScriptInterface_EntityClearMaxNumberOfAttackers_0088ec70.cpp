// CGameScriptInterface::EntityClearMaxNumberOfAttackers(const CScriptThing&) const -- removes the attacker limit: EntitySetMaxNumberOfAttackers(thing, -1)
// Leaf: EntityClearMaxNumberOfAttackers@CGameScriptInterface
// Retail: Fable.exe @ 0x0088EC70, 18 bytes, `ret 4`; calls its own vtable slot 463 (+0x73c), EntitySetMaxNumberOfAttackers(const CScriptThing&, long)
// (ghidra_out/cgsi_vtable_slots.tsv). The vtable is modelled in slot order with placeholder virtuals.
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntityClearMaxNumberOfAttackers(const CScriptThing& entity) const
{
    EntitySetMaxNumberOfAttackers(entity, -1);
}