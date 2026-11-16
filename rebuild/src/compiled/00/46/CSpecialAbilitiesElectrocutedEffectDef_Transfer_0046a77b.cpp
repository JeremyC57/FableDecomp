// CSpecialAbilitiesElectrocutedEffectDef::Transfer(CPersistContext& context) -- persist the def's members in declaration order
// Leaf: ?Transfer@CSpecialAbilitiesElectrocutedEffectDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046A77B, 101 bytes, virtual `ret 4` (RTTI vtable slot 18; RTTI base CParentDefClassBase): Transfer<float> 0x00431061 x3, TransferVector<long> 0x00466A47 x2, Transfer<float> x3.
// Port (round 4, NF40): was a stand-in (leaf ?Transfer@CSpecialAbilitiesElectrocutedEffectDef@@UAEXAAVCPersistContext@@@Z) on a local CPersistContext model with stand-in per-type transfer members
//   over opaque fields; now engine/CPersistContext.h (Transfer<T> / TransferVector<T>) and the retail def chain
//   engine/CParentDefClassBase.h. No shared header exists for CSpecialAbilitiesElectrocutedEffectDef, so the row keeps its own model of the class, with the
//   Ego_r.pdb members (FadeInTimeSecs +0x3C, FadeOutTimeStartSecs +0x40, FadeOutTimeSecs +0x44, Color +0x48, HighlightColor +0x54, HighlightWidth +0x60, HeightSmall +0x64, HeightMedium +0x68; the retail offsets are the Ego_r ones minus 4 after the 0x39-byte retail CParentDefClassBase) and the virtual override
//   ?Transfer@CSpecialAbilitiesElectrocutedEffectDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb).
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"

class CSpecialAbilitiesElectrocutedEffectDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    float                      FadeInTimeSecs;                // +0x3C
    float                      FadeOutTimeStartSecs;          // +0x40
    float                      FadeOutTimeSecs;               // +0x44
    std::vector<long>          Color;                         // +0x48
    std::vector<long>          HighlightColor;                // +0x54
    float                      HighlightWidth;                // +0x60
    float                      HeightSmall;                   // +0x64
    float                      HeightMedium;                  // +0x68
};

void CSpecialAbilitiesElectrocutedEffectDef::Transfer(CPersistContext& context)
{
    context.Transfer(FadeInTimeSecs);
    context.Transfer(FadeOutTimeStartSecs);
    context.Transfer(FadeOutTimeSecs);
    context.TransferVector(Color);
    context.TransferVector(HighlightColor);
    context.Transfer(HighlightWidth);
    context.Transfer(HeightSmall);
    context.Transfer(HeightMedium);
}
