// COpinionPersonalityTraits::COpinionPersonalityTraits() -- default constructor: zero base offset, every scaling factor 1.0
// Leaf: ??0COpinionPersonalityTraits
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C26D, length 47 bytes, plain `ret` (thiscall, returns this in eax).
// Catalog label "InitializeFloatArray_Ones" is behavioural: the only reference is the `vector constructor iterator`
// call in COpinionPersonalityDef::COpinionPersonalityDef (landed 0x00456F71, `new COpinionPersonalityTraits[5]`).
// Layout (PDB COpinionPersonalityTraits, 0x24 bytes, nine floats): BaseOffset +0x00, HeroStatScaling +0x04,
// VillageOpinionScaling +0x08, TimeScalingRunIn +0x0C, TimeScalingRunOut +0x10, PeakScalingIfPos +0x14,
// PeakScalingIfNeg +0x18, PersistScalingIfPos +0x1C, PersistScalingIfNeg +0x20. No engine header exists for it.
// Shape: `fldz ; fstp` / `fld1 ; fstp` for the float constants (instead of immediate dword stores) marks a size TU:
// #pragma optimize("s", on). The initialiser list follows declaration order.
#pragma optimize("s", on)
class COpinionPersonalityTraits {
public:
    float BaseOffset;               // +0x00
    float HeroStatScaling;          // +0x04
    float VillageOpinionScaling;    // +0x08
    float TimeScalingRunIn;         // +0x0C
    float TimeScalingRunOut;        // +0x10
    float PeakScalingIfPos;         // +0x14
    float PeakScalingIfNeg;         // +0x18
    float PersistScalingIfPos;      // +0x1C
    float PersistScalingIfNeg;      // +0x20
    COpinionPersonalityTraits();
};

COpinionPersonalityTraits::COpinionPersonalityTraits()
    : BaseOffset(0.0f), HeroStatScaling(1.0f), VillageOpinionScaling(1.0f), TimeScalingRunIn(1.0f),
      TimeScalingRunOut(1.0f), PeakScalingIfPos(1.0f), PeakScalingIfNeg(1.0f), PersistScalingIfPos(1.0f),
      PersistScalingIfNeg(1.0f)
{
}