// CCreatureModeDef::Transfer(CPersistContext& context) -- persist the def's members in declaration order
// Leaf: ?Transfer@CCreatureModeDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7EA7, 57 bytes, virtual `ret 4` (RTTI vtable slot 18; RTTI base CSubDefClassBase): Transfer<long> 0x00431102 x3, TransferVector<long> 0x00466A47.
// Port (round 4, NF40): was a stand-in (leaf ?CCreatureModeDef_Transfer@@YIXPAVCCarrySlotDef@@PAXPAVCPersistContext@@@Z) on a local CPersistContext model with stand-in per-type transfer members
//   over opaque fields; now engine/CPersistContext.h (Transfer<T> / TransferVector<T>) and the retail def chain
//   engine/CSubDefClassBase.h. No shared header exists for CCreatureModeDef, so the row keeps its own model of the class, with the
//   Ego_r.pdb members (DefaultCreatureMode +0x28, InitialCreatureMode +0x2C, DefaultWeaponCreatureMode +0x30, AvailableIdleModes +0x34; the retail offsets are the Ego_r ones) and the virtual override
//   ?Transfer@CCreatureModeDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb).
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"

class CCreatureModeDef : public CSubDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    long                       DefaultCreatureMode;           // +0x28
    long                       InitialCreatureMode;           // +0x2C
    long                       DefaultWeaponCreatureMode;     // +0x30
    std::vector<long>          AvailableIdleModes;            // +0x34
};

void CCreatureModeDef::Transfer(CPersistContext& context)
{
    context.Transfer(DefaultCreatureMode);
    context.Transfer(InitialCreatureMode);
    context.Transfer(DefaultWeaponCreatureMode);
    context.TransferVector(AvailableIdleModes);
}
