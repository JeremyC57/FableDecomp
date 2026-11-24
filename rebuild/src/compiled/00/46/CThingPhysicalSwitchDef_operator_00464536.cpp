// CThingPhysicalSwitchDef::operator=(const CThingPhysicalSwitchDef& other) -- memberwise def assignment: CThingBaseDef's out-of-line operator=, then TriggerType, the two CEngineGraphic records and TriggerSound; returns *this
// Leaf: ??4CThingPhysicalSwitchDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00464536, 71 bytes, thiscall, `ret 4`.
// Identity: CThingPhysicalSwitchDef::Copy 0x00464531 (slot 19 of the class's RTTI vftable; RTTI placement + both PDB name sets) is a `jmp` to
//   it; ??4CThingPhysicalSwitchDef@@QAEAAV0@ABV0@@Z is in Ego_r.pdb and FableWin.pdb. The copied members after the base call are the class's
//   own (engine/CThingPhysicalSwitchDef.h, Ego_r.pdb order less 4): TriggerType +0x70, UntriggeredGraphic +0x74, TriggeredGraphic +0x82 (two
//   byte-packed 14-byte CEngineGraphic: `movsd x3 ; movsw`), TriggerSound +0x90; the 0x94-byte object is this class, not CThingBaseDef (0x70).
// Callees: CThingBaseDef::operator=(const CThingBaseDef&) 0x00463950.
// NF337 (2026-11-24): respelled from ??4CThingBaseDef (the 0x00463950 row's own symbol: one retail address, one name) and the made-up base
//   CThingBaseDefBase onto the shared class model; FABLE_RETAIL_CTHINGBASEDEF_COPY_DECLS declares the base operator= out of line and
//   FABLE_RETAIL_CTHINGPHYSICALSWITCHDEF_COPY_DECLS this one.
// Pragma: `#pragma optimize("s", on)` (unaligned start; the speed form copies the records through eax/ecx dwords).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBASEDEF_COPY_DECLS
#define FABLE_RETAIL_CTHINGPHYSICALSWITCHDEF_COPY_DECLS
#include "engine/CThingPhysicalSwitchDef.h"

CThingPhysicalSwitchDef& CThingPhysicalSwitchDef::operator=(const CThingPhysicalSwitchDef& other)
{
    CThingBaseDef::operator=(other);
    TriggerType = other.TriggerType;
    UntriggeredGraphic = other.UntriggeredGraphic;
    TriggeredGraphic = other.TriggeredGraphic;
    TriggerSound = other.TriggerSound;
    return *this;
}
