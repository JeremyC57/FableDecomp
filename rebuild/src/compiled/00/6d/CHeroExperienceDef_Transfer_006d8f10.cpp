// CHeroExperienceDef::Transfer(CPersistContext& context) -- persist the hero experience definition (slot 18 of vtable 0x0124390C): level thresholds, level titles, fallback multipliers, sound-intensity and combo-multiplier maps, the two combo experience multipliers, the stat-increase-per-hit-type map, the four combo sound maps, the stat map again, and the attack-history combos
// Leaf: ?Transfer@CHeroExperienceDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D8F10, 692 bytes, __fastcall virtual, `ret 4`. Name: Ego_r.pdb 0058a900 + FableWin.pdb 017a57c4 + vtable slot 18.
// Callees: CPersistContext::CheckCRC 0x00404500 (x14, pooled "" 0x0122D70E);
//   GFSerialiseVectorBinaryOut/In<long> 0x00466A75 / 0x00466AA7; GFSerialiseMapBinaryOut/In<CDefString,long,less<long>> 0x006DAE40 / 0x006DBA50;
//   (0x00466A75 is the <long> writer, FableWin.pdb ??$GFSerialiseVectorBinaryOut@J@@YIXAAVCDataOutputStream@@AAV?$vector@JV?$allocator@J@std@@@std@@@Z:
//   it writes the count and every element through CDataOutputStream::WriteSLONG 0x00993EB0 and never calls WriteFloat 0x00993EE0, it is
//   paired here with the <long> reader 0x00466AA7 on the same three members, and Ego_r types those members vector<long>; the manifest's
//   <float> label for it is a propagated one)
//   GFSerialiseMapBinaryOut/In<CComboMultiplierData,long,less<long>> 0x006DC350 / 0x006DCFA0;
//   GFSerialiseMapBinaryOut/In<CHeroStatIncreaseData,EDamageAttribute,less<EDamageAttribute>> 0x006DC450 / 0x006DD1C0;
//   GFSerialiseVectorBinaryOut/In<CAttackHistoryCombo> 0x006DC550 / 0x006DD580 (all __fastcall stream&, container&);
//   CDataOutputStream::WriteFloat 0x00993EE0, CPersistTraits<float>::TransferBinaryIn 0x0040EFB0.
// Shape: fourteen inline one-argument CPersistContext transfers as TLC's release persist macros call them. TransferMap and
//   Transfer<float> switch on the two binary modes and skip the rest (`sub eax,2 ; je ; dec eax ; jne`); TransferVector's switch has
//   an unreachable default (`sub eax,2 ; je` only). StatIncreasePerHitType is transferred twice (8th and 13th, as in Ego_r and the def
//   schema); its address is the one value held across the four combo maps, in ebp.
// Why ebp is saved mid-body (`push ebp` 0x006D9064 before the first stat transfer, `pop ebp` 0x006D91A3 in the last vector's head):
//   the vector default is a braced `{ __assume(0); }` (a NODEFAULT-style macro). VC7.1 keeps that dead-end block until after register
//   allocation, so ebx -- referenced by the three vector transfers at the top -- has no block that post-dominates its uses and its
//   restore stays in the epilogue; ebp's save can then nest inside it, around the blocks that use it. With a bare `__assume(0);`
//   (pruned early) or a `default:` that writes, ebx's restore is hoistable to the head of the second stat transfer, the two ranges
//   cross, and ebp is saved in the prologue instead (693 bytes).
// Headers: overlay opt-ins FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER, FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERMAP_BINARY_BODY,
//   FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NODEFAULT, FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS (see hdr/README.md).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NODEFAULT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERMAP_BINARY_BODY
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CATTACKHISTORYCOMBO
#define FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CHeroExperienceDef.h"

void CHeroExperienceDef::Transfer(CPersistContext& context)
{
    context.TransferVector(ExperienceLevelThresholds);
    context.TransferVector(HeroLevelTitles);
    context.TransferVector(FallbackMultipliers);
    context.TransferMap(SoundIntensityThresholds);
    context.TransferMap(ComboMultiplierMap);
    context.Transfer(ExperienceMultiplierParamPerUnitComboMultiplier);
    context.Transfer(ExperienceMultiplierRegionChangeMultiplier);
    context.TransferMap(StatIncreasePerHitType);
    context.TransferMap(ComboMultiplierIncrease);
    context.TransferMap(ComboMultiplierLock);
    context.TransferMap(ComboMultiplierLost);
    context.TransferMap(ComboMultiplierRunningHitsIncrease);
    context.TransferMap(StatIncreasePerHitType);
    context.TransferVector(AttackHistoryCombos);
}