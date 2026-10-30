// CTCWeapon::IsAbleToBeUsedToBlock(const CThing& user, const CThing* attacker, const CThing* attack) -- a weapon can block when its user may wield it
// Grade: DIFFER(19v22) -- VC7.1 emits setne/neg for the bool result where retail keeps two exits (`test al,al ; jne T ; xor al,al ; ret 0Ch ; T: mov al,1 ; ret 0Ch`). 3 grades.
// Leaf: ?IsAbleToBeUsedToBlock@CTCWeapon@@QBE_NABVCThing@@PBV2@1@Z
// Retail: Fable.exe 0x005DACF0, 22 bytes, __thiscall `ret 0Ch`. Calls CTCWeapon::IsUsableBy(const CThing&) const 0x005DAB10.
// Names: Ego_r.pdb `?IsAbleToBeUsedToBlock@CTCWeapon@@QBE_NABVCThing@@PBV2@1@Z` / `?IsUsableBy@CTCWeapon@@QBE_NABVCThing@@@Z`
//   (both PDBs); the `ret 0Ch` matches the three parameters, only the first is read.
// Shape: the false exit returns the callee's al (already 0) and the true exit loads 1: the bool result kept in a named local
//   and returned on the false path.
#define CTCWeapon CTCWeaponPdb
#include "engine/CTCWeapon.h"
#undef CTCWeapon

class CThing;

class CTCWeapon : public CTCWeaponPdb
{
public:
    bool IsUsableBy(const CThing& user) const;                                                       // 0x005DAB10
    bool IsAbleToBeUsedToBlock(const CThing& user, const CThing* attacker, const CThing* attack) const;
};

bool CTCWeapon::IsAbleToBeUsedToBlock(const CThing& user, const CThing* attacker, const CThing* attack) const
{
    bool usable = IsUsableBy(user);
    if (usable)
        usable = true;
    return usable;
}
