// CSpecialAbilitiesBurntEffectDef::Transfer(CPersistContext& context) -- persist the def's members in declaration order
// Leaf: ?Transfer@CSpecialAbilitiesBurntEffectDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046A742, 57 bytes, virtual `ret 4` (RTTI vtable slot 18; RTTI base CParentDefClassBase): Transfer<float> 0x00431061 x3, TransferVector<long> 0x00466A47.
// Port (round 4, NF40): was a stand-in (leaf ?CSpecialAbilitiesBurntEffectDef_Transfer@@YIXPAVCCameraManagerSetDef@@PAXPAVCPersistContext@@@Z) on a local CPersistContext model with stand-in per-type transfer members
//   over opaque fields; now engine/CPersistContext.h (Transfer<T> / TransferVector<T>) and the retail def chain
//   engine/CParentDefClassBase.h. No shared header exists for CSpecialAbilitiesBurntEffectDef, so the row keeps its own model of the class, with the
//   Ego_r.pdb members (FadeInTimeSecs +0x3C, FadeOutTimeStartSecs +0x40, FadeOutTimeSecs +0x44, Color +0x48; the retail offsets are the Ego_r ones minus 4 after the 0x39-byte retail CParentDefClassBase) and the virtual override
//   ?Transfer@CSpecialAbilitiesBurntEffectDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb).
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"

class CSpecialAbilitiesBurntEffectDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    float                      FadeInTimeSecs;                // +0x3C
    float                      FadeOutTimeStartSecs;          // +0x40
    float                      FadeOutTimeSecs;               // +0x44
    std::vector<long>          Color;                         // +0x48
};

void CSpecialAbilitiesBurntEffectDef::Transfer(CPersistContext& context)
{
    context.Transfer(FadeInTimeSecs);
    context.Transfer(FadeOutTimeStartSecs);
    context.Transfer(FadeOutTimeSecs);
    context.TransferVector(Color);
}
