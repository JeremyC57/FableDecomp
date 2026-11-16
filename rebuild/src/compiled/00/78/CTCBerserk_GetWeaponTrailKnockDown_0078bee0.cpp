// Leaf: ?GetWeaponTrailKnockDown@CTCBerserk@@QBEJXZ
// CTCBerserk::GetWeaponTrailKnockDown() const -- the berserk def's WeaponTrailKnockDown value (PDef +0x68, def +0x14c).
// NF35 (2026-11-16): renamed from the placeholder ?M@C_0078bee0@@QAEKXZ (struct stand-ins with pads) to the Ego_r.pdb + FableWin.pdb
//   ?GetWeaponTrailKnockDown@CTCBerserk@@QBEJXZ and moved onto engine/CTCBerserk.h (WEAPONTRAIL opt-in, landed by row 0x00839100, which calls it)
//   + engine/CSpecialAbilitiesBerserkDef.h. Retail: Fable.exe @ 0x0078BEE0: mov eax,[ecx+68h]; mov eax,[eax+14ch]; ret.
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

long CTCBerserk::GetWeaponTrailKnockDown() const
{
    return PDef->WeaponTrailKnockDown;
}
