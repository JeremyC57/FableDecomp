// C3DAnimationBuilder::AssignAnimation(CBoneInfo&, const CCountedPointer<CBoneInfo>&, bool) -- pairs destination and source bones recursively.
// Leaf: ?AssignAnimation@C3DAnimationBuilder@@AAEKAAVCBoneInfo@1@ABV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@_N@Z
// Flags: default
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#include "engine/C3DAnimationBuilder.h"


long C3DAnimationBuilder::CBoneInfo::FindChildIndex(const CCharString& name) const
{
    for (unsigned long child = 0; child < Children.size(); ++child) {
        if (name == Children[child]->Name)
            return child;
    }
    return -1;
}

unsigned long C3DAnimationBuilder::AssignAnimation(CBoneInfo& dest, const CCountedPointer<CBoneInfo>& src, bool skip_levels)
{
    unsigned long matches = 0;
    dest.AnimSource = src;
    if (skip_levels && dest.OriginalNoChildren == 1 && src->OriginalNoChildren == 1) {
        ++matches;
        if (!dest.Children.empty() && !src->Children.empty()) {
            CBoneInfo& dest_child = *dest.Children[0];
            bool added_bone = false;
            CCharString lc_name = dest_child.Name.GetAsLowerCase();
            for (unsigned long c_prefix = 0; c_prefix < AddedBonePrefixes.size(); ++c_prefix) {
                if (lc_name.StartsWith(AddedBonePrefixes[c_prefix]))
                    added_bone = true;
            }
            if (added_bone) {
                unsigned long test_normal = AssignAnimation(dest_child, src->Children[0], skip_levels);
                unsigned long test_skip = AssignAnimation(dest_child, src, skip_levels);
                if (test_skip > test_normal) {
                    matches += AssignAnimation(dest_child, src, skip_levels);
                    dest.ExtensionParent = true;
                    dest_child.ExtensionChild = true;
                    dest_child.AnimSource.Reset(0);
                } else {
                    matches += AssignAnimation(dest_child, src->Children[0], skip_levels);
                }
            } else {
                matches += AssignAnimation(dest_child, src->Children[0], skip_levels);
            }
        }
    } else {
        for (unsigned long c_child = 0; c_child < dest.Children.size(); ++c_child) {
            CBoneInfo& dest_child = *dest.Children[c_child];
            long index = src->FindChildIndex(dest_child.Name);
            if (index != -1) {
                ++matches;
                matches += AssignAnimation(dest_child, src->Children[index], skip_levels);
            }
        }
    }
    return matches;
}