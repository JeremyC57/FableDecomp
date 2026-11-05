// CTCWeapon::IsAbleToBeUsedToBlock(const CThing& blocker, const CThing* attacker, const CThing* attackerWeapon) const -- whether `blocker` can block with this weapon
// Leaf: ?IsAbleToBeUsedToBlock@CTCWeapon@@QBE_NABVCThing@@PBV2@1@Z
// Flags: none
// Retail: Fable.exe @ 0x005DACF0, 22 bytes (+ int3 padding), thiscall `ret 0Ch`; one caller (CTCCombat, 0x00735039).
// Names: Ego_r.pdb 0x0063AAB0 and FableWin.pdb ?IsAbleToBeUsedToBlock@CTCWeapon@@QBE_NABVCThing@@PBV2@1@Z (bool, const);
// the same (const CThing&, const CThing*, const CThing*) signature as the landed sibling IsBlockBrokenOnBlock (0x005DAD10).
// Callee: CTCWeapon::IsUsableBy(const CThing&) const @ 0x005DAB10 (engine/CTCWeapon.h).
// Body: a weapon the blocker cannot use never blocks; otherwise the answer is true whether or not an attacker is given.
// Shape: the `attacker == NULL` early-out, whose two arms both return true, is what keeps retail's branch form
// (`test al,al; jne; ret 0Ch; mov al,1; ret 0Ch`): VC7.1 merges the two true arms only after it has laid out the
// branches, so the false path returns the call's own zero al. Without it, every spelling of the one condition becomes
// `setne al` (asm round 2b, 60 spellings). /O2 TU (int3 padding to 16 around the siblings).
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_BLOCK
#include "engine/CTCWeapon.h"

bool CTCWeapon::IsAbleToBeUsedToBlock(const CThing& blocker, const CThing* attacker, const CThing* attackerWeapon) const
{
    if (!IsUsableBy(blocker))
        return false;
    if (attacker == NULL)
        return true;
    return true;
}
