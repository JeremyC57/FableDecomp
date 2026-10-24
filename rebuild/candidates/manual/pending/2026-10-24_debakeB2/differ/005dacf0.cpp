// Grade: DIFFER(18v22) -- unchanged best (debakeB); VC7.1 folds the bool return into test al,al/setne al where retail keeps the two-exit branch (false: ret 0Ch on al==0, true: mov al,1). debakeB2 tried 6 more: /Os if(!x) return false and if(x) return true (both 17v22), a dead second test, a switch on the bool, a goto to a true label, while(usable) return true (all 18v22). Likely the original had a second, constant-folded condition before the true exit.
// CTCWeapon::IsAbleToBeUsedToBlock(CThing* user, CThing* attacker, CThing* attack) -- a weapon can block when its user may wield it
// Retail: Fable.exe @ 0x005DACF0, 22 bytes, __thiscall `ret 0Ch` (three CThing* arguments, only the first used).
// Forwards the user to CTCWeapon::IsUsableBy(CThing*) (0x005DAB10, 13 callers) on the same `this`; the false exit
// re-uses al == 0 from the test, the true exit loads 1 -- the `if (!x) return false; return true;` shape.
// Caller 0x00735039 passes (ebp, [esp+24h], [esp+28h]).
#include "engine/CTCWeapon.h"
class CThing;
struct CTCWeaponBlockQuery : CTCWeapon {
    bool IsUsableBy(CThing* user) const;                                            // 0x005DAB10
    bool IsAbleToBeUsedToBlock(CThing* user, CThing* attacker, CThing* attack) const;
};
bool CTCWeaponBlockQuery::IsAbleToBeUsedToBlock(CThing* user, CThing* attacker, CThing* attack) const
{
    if (!IsUsableBy(user))
        return false;
    return true;
}
