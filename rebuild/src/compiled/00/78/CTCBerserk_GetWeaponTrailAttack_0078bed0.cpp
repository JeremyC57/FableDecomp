// Leaf: ?GetWeaponTrailAttack@CTCBerserk@@QBEJXZ
// CTCBerserk::GetWeaponTrailAttack() const -- the berserk def's WeaponTrailAttack value (PDef +0x68, def +0x148).
// NF35 (2026-11-16): renamed from the placeholder ?M@C_0078bed0@@QAEKXZ (struct stand-ins with pads) to the Ego_r.pdb + FableWin.pdb
//   ?GetWeaponTrailAttack@CTCBerserk@@QBEJXZ and moved onto engine/CTCBerserk.h (WEAPONTRAIL opt-in, landed by row 0x00839100, which calls it)
//   + engine/CSpecialAbilitiesBerserkDef.h. Retail: Fable.exe @ 0x0078BED0: mov eax,[ecx+68h]; mov eax,[eax+148h]; ret.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCBERSERK_WEAPONTRAIL
#include "engine/CTCBerserk.h"
#include "engine/CSpecialAbilitiesBerserkDef.h"

long CTCBerserk::GetWeaponTrailAttack() const
{
    return PDef->WeaponTrailAttack;
}
