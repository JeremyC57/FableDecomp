// CTCCreatureOpinionOfHero::TransformForcedOpinion(EOpinion opinion, float value) -- scale a forced opinion value by the creature's personality traits for that opinion (BaseOffset + value * HeroStatScaling); unchanged when no personality def is set
// Leaf: ?TransformForcedOpinion@CTCCreatureOpinionOfHero@@QBEMW4EOpinion@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681790, 43 bytes (`ret 8`), no callees.
// Name: Ego_r.pdb (donor 0x0058F830) and FableWin.pdb (0x01F42F70) list ?TransformForcedOpinion@CTCCreatureOpinionOfHero@@
//   QBEMW4EOpinion@@M@Z immediately before ?TransformOpinionFromHeroStats@..@@QBEMW4EOpinion@@M@Z; retail has the two
//   byte-identical bodies in that order (0x00681790, landed twin 0x006817C0). The sole caller 0x00700F1C (COpinionReactionManager)
//   passes a forced value read from an array, while both callers of 0x006817C0 pass GetHeroStatsInOpinionFormat's result.
// Source: the landed twin 0x006817C0 body verbatim (ChatGPT pass 10's CParentDefClassBase/named-reference form missed the
//   reload, 40v43); the accessor called at each use is what keeps IsNull's load apart from the operator-> reload.
// Shape: the def pointer is tested through CDefPointer::IsNull (eax) and re-read for the computation through operator->
//   (ecx = def, edx = PersonalityTraits): the inline IsNull result is not CSE'd with the operator-> read. The non-null path is
//   the fall-through (`if (!IsNull()) return ...; return value;`). PersonalityTraits is a COpinionPersonalityTraits array
//   (36-byte records: `lea eax,[eax+eax*8]` then *4); `value * HeroStatScaling` is evaluated first (fld value; fmul), then
//   BaseOffset is added.
// Layout: `this` from engine/CTCCreatureOpinionOfHero.h (retail STLport offsets; the header flattens the CDefPointer at +0x20 to
//   `PPersonalityDef_Object`, so the accessor below views it as the real CDefPointer<const COpinionPersonalityDef>, the
//   pattern of the landed CTCWeapon::SetProjectileWeaponLoaded 0x005DAC50). COpinionPersonalityDef derives from the retail
//   CDefClassBase (0x25 packed); Ego_r.pdb PersonalityTraits at +0x40 sits at retail +0x3c (the Ego_r def root is 4 bytes
//   longer). COpinionPersonalityTraits members are the Ego_r.pdb names (36 bytes).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#define CTCCreatureOpinionOfHero CTCCreatureOpinionOfHeroLayout
#include "engine/CTCCreatureOpinionOfHero.h"
#undef CTCCreatureOpinionOfHero

enum EOpinion { };                                  // W4EOpinion@@ (values unused here)

class COpinionPersonalityTraits { public:           // Ego_r.pdb, 36 bytes
    float BaseOffset;                               // +0x00
    float HeroStatScaling;                          // +0x04
    float VillageOpinionScaling;                    // +0x08
    float TimeScalingRunIn;                         // +0x0c
    float TimeScalingRunOut;                        // +0x10
    float PeakScalingIfPos;                         // +0x14
    float PeakScalingIfNeg;                         // +0x18
    float PersistScalingIfPos;                      // +0x1c
    float PersistScalingIfNeg;                      // +0x20
};

class COpinionPersonalityDef : public CDefClassBase { public:
    unsigned char _pad25[0x3c - 0x25];
    COpinionPersonalityTraits* PersonalityTraits;   // +0x3c (indexed by EOpinion)
};

class CTCCreatureOpinionOfHero : public CTCCreatureOpinionOfHeroLayout { public:
    const CDefPointer<const COpinionPersonalityDef>& PPersonalityDef() const
    {
        return *reinterpret_cast<const CDefPointer<const COpinionPersonalityDef>*>(&PPersonalityDef_Object);
    }
    float TransformForcedOpinion(EOpinion opinion, float value) const;
};

float CTCCreatureOpinionOfHero::TransformForcedOpinion(EOpinion opinion, float value) const
{
    if (!PPersonalityDef().IsNull())
        return value * PPersonalityDef()->PersonalityTraits[opinion].HeroStatScaling
             + PPersonalityDef()->PersonalityTraits[opinion].BaseOffset;
    return value;
}