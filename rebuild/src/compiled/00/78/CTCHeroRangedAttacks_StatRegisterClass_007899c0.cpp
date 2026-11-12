// CTCHeroRangedAttacks::StatRegisterClass() -- reset the class's static accuracy override to -1.0f (no override)
// Leaf: ?StatRegisterClass@CTCHeroRangedAttacks@@SIXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007899C0, 11 bytes: `mov dword ptr [0x013BAE04], 0BF800000h ; ret`.
// Identity (proven, agentFAM07): CTCBase::RegisterTCs 0x004EE23F calls it (0x004EE272) in its run of per-component
//   StatRegisterClass calls (CTCCreatureGenerator 0x00749D60, CTCCombat 0x007330F0, ...) and the landed RegisterTCs binds this
//   address as ?StatRegisterClass@CTCHeroRangedAttacks@@SIXXZ (reloc_bindings); the name is in both PDB name sets (public static
//   __fastcall). Its only store is to 0x013BAE04, the private static CTCHeroRangedAttacks::AccuracyOverride (?AccuracyOverride@
//   CTCHeroRangedAttacks@@0MA, bound by the landed FireRangedWeapon 0x0078A390, which reads it). Was the placeholder
//   ResetUnprovenFloat_007899c0 with an invented global.
// Headers: engine/CTCHeroRangedAttacks.h (FABLE_RETAIL_FIRE_RANGED_WEAPON branch declares both members).
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroRangedAttacks.h"

void FABLE_FASTCALL CTCHeroRangedAttacks::StatRegisterClass()
{
    AccuracyOverride = -1.0f;
}
