// 00a45150 C3DSkeletalMorphTarget::Initialise
// Flags: /Gr
// Leaf: ?Initialise@C3DSkeletalMorphTarget@@QAEXABVCCharString@@ABV?$CArray@VC3DSkeletalBoneMorph@@@@@Z
#define FABLE_RETAIL_STLP_SORT_INLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_SKELETAL_MORPH_FILE_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "fable_string.h"
#include "engine/C3DSkeletalMorph.h"

void C3DSkeletalMorphTarget::Initialise(const CCharString& name, const CArray<C3DSkeletalBoneMorph>& morph_list)
{
    Name = name;
    for (unsigned long c_morph = 0; c_morph < morph_list.size(); ++c_morph) {
        const C3DSkeletalBoneMorph& morph = morph_list[c_morph];
        if (morph.GetScaling().X != 1.0f || morph.GetScaling().Y != 1.0f || morph.GetScaling().Z != 1.0f)
            BoneMorphs.push_back(morph);
    }
    std::sort(BoneMorphs.begin(), BoneMorphs.end(), C3DSkeletalBoneMorph::CompareCRCs);
}
