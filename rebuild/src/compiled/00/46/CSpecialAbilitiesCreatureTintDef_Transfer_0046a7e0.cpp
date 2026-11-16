// CSpecialAbilitiesCreatureTintDef::Transfer(CPersistContext& context) -- persist the def's members in declaration order
// Leaf: ?Transfer@CSpecialAbilitiesCreatureTintDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046A7E0, 46 bytes, virtual `ret 4` (RTTI vtable slot 18; RTTI base CParentDefClassBase): Transfer<float> 0x00431061 x2, TransferVector<long> 0x00466A47.
// Port (round 4, NF40): was a stand-in (leaf ?CSpecialAbilitiesCreatureTintDef_Transfer@@YIXPAVCVillagerInteractionsDef@@PAXPAVCPersistContext@@@Z) on a local CPersistContext model with stand-in per-type transfer members
//   over opaque fields; now engine/CPersistContext.h (Transfer<T> / TransferVector<T>) and the retail def chain
//   engine/CParentDefClassBase.h. No shared header exists for CSpecialAbilitiesCreatureTintDef, so the row keeps its own model of the class, with the
//   Ego_r.pdb members (FadeInTimeSecs +0x3C, FadeOutTimeSecs +0x40, Color +0x44; the retail offsets are the Ego_r ones minus 4 after the 0x39-byte retail CParentDefClassBase) and the virtual override
//   ?Transfer@CSpecialAbilitiesCreatureTintDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb).
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"

class CSpecialAbilitiesCreatureTintDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    float                      FadeInTimeSecs;                // +0x3C
    float                      FadeOutTimeSecs;               // +0x40
    std::vector<long>          Color;                         // +0x44
};

void CSpecialAbilitiesCreatureTintDef::Transfer(CPersistContext& context)
{
    context.Transfer(FadeInTimeSecs);
    context.Transfer(FadeOutTimeSecs);
    context.TransferVector(Color);
}
