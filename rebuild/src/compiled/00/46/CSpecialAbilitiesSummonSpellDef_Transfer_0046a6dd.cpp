// CSpecialAbilitiesSummonSpellDef::Transfer(CPersistContext& context) -- persist the def's members in declaration order
// Leaf: ?Transfer@CSpecialAbilitiesSummonSpellDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046A6DD, 101 bytes, virtual `ret 4` (RTTI vtable slot 18; RTTI base CParentDefClassBase): TransferVector<float> 0x00464CBF, Transfer<CDefString> 0x0045228F, Transfer<float> 0x00431061 x2, Transfer<long> 0x00431102, TransferVector<long> 0x00466A47, Transfer<CCharString> 0x00431143 x2.
// Port (round 4, NF40): was a stand-in (leaf ?Transfer@CSpecialAbilitiesSummonSpellDef@@UAEXAAVCPersistContext@@@Z) on a local CPersistContext model with stand-in per-type transfer members
//   over opaque fields; now engine/CPersistContext.h (Transfer<T> / TransferVector<T>) and the retail def chain
//   engine/CParentDefClassBase.h. No shared header exists for CSpecialAbilitiesSummonSpellDef, so the row keeps its own model of the class, with the
//   Ego_r.pdb members (TimeOfSummon +0x3C, BaseSummonCreatureName +0x48, FadeInTime +0x4C, FadeOutTime +0x50, ResetCreatureManaCost +0x54, SummonTintColour +0x58, EyeLeftDummyPointName +0x64, EyeRightDummyPointName +0x68; the retail offsets are the Ego_r ones minus 4 after the 0x39-byte retail CParentDefClassBase) and the virtual override
//   ?Transfer@CSpecialAbilitiesSummonSpellDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb).
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"

class CSpecialAbilitiesSummonSpellDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    std::vector<float>         TimeOfSummon;                  // +0x3C
    CDefString                 BaseSummonCreatureName;        // +0x48
    float                      FadeInTime;                    // +0x4C
    float                      FadeOutTime;                   // +0x50
    long                       ResetCreatureManaCost;         // +0x54
    std::vector<long>          SummonTintColour;              // +0x58
    CCharString                EyeLeftDummyPointName;         // +0x64
    CCharString                EyeRightDummyPointName;        // +0x68
};

void CSpecialAbilitiesSummonSpellDef::Transfer(CPersistContext& context)
{
    context.TransferVector(TimeOfSummon);
    context.Transfer(BaseSummonCreatureName);
    context.Transfer(FadeInTime);
    context.Transfer(FadeOutTime);
    context.Transfer(ResetCreatureManaCost);
    context.TransferVector(SummonTintColour);
    context.Transfer(EyeLeftDummyPointName);
    context.Transfer(EyeRightDummyPointName);
}
