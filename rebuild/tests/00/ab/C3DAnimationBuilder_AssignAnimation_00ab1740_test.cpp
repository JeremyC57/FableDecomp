#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define private public
#define protected public
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

#undef protected
#undef private
#include <ctype.h>
#include <stdio.h>
CCharString::CCharString() : PStringData(0) {}
CCharString::CCharString(const char* text, long length) : PStringData(new CCharStringData)
{
    if (length < 0)
        length = strlen(text);
    PStringData->NoRefs = 1;
    PStringData->Data.PData = new char[length + 1];
    PStringData->Data.StringLength = length;
    PStringData->Data.DataLength = length + 1;
    PStringData->Data.UseFastExtend = false;
    memcpy(PStringData->Data.PData, text, length);
    PStringData->Data.PData[length] = 0;
}
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData)
{
    if (PStringData)
        ++PStringData->NoRefs;
}
CCharString::~CCharString()
{
    if (PStringData && --PStringData->NoRefs == 0) {
        delete[] PStringData->Data.PData;
        delete PStringData;
    }
}
static const char* NarrowText(const CCharString& text)
{
    return text.PStringData ? text.PStringData->Data.PeekData() : "";
}
CCharString CCharString::GetAsLowerCase() const
{
    const char* source = NarrowText(*this);
    char lower[128];
    const size_t n = strlen(source);
    if (n >= sizeof(lower))
        abort();
    for (size_t i = 0; i < n; ++i)
        lower[i] = (char)tolower((unsigned char)source[i]);
    lower[n] = 0;
    return CCharString(lower);
}

const char CCharString::NullChar = 0;
const CCharString& CCharString::operator=(const CCharString& other)
{
    if (other.PStringData != PStringData) {
        if (other.PStringData)
            ++other.PStringData->NoRefs;
        if (PStringData && --PStringData->NoRefs == 0) {
            delete[] PStringData->Data.PData;
            delete PStringData;
        }
        PStringData = other.PStringData;
    }
    return *this;
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
C3DAnimationBuilder::C3DAnimationBuilder() {}
C3DAnimationBuilder::CBoneInfo::~CBoneInfo() {}

typedef C3DAnimationBuilder::CBoneInfo Bone;
typedef CCountedPointer<Bone> BonePtr;
static BonePtr MakeBone(const char* name, unsigned long children = 0)
{
    BonePtr p(new Bone);
    p->Name = CCharString(name);
    p->OriginalNoChildren = children;
    return p;
}

int main()
{
    C3DAnimationBuilder builder;
    builder.AddedBonePrefixes.push_back(CCharString("added_"));

    {
        BonePtr src = MakeBone("source", 2);
        BonePtr srcMatch = MakeBone("A");
        src->Children.push_back(srcMatch);
        src->Children.push_back(MakeBone("B"));
        BonePtr dest = MakeBone("dest", 2);
        BonePtr destMatch = MakeBone("A");
        BonePtr destUnmatched = MakeBone("C");
        dest->Children.push_back(destMatch);
        dest->Children.push_back(destUnmatched);
        if (builder.AssignAnimation(*dest, src, false) != 1 || dest->AnimSource.Get() != src.Get()
            || destMatch->AnimSource.Get() != srcMatch.Get() || !destUnmatched->AnimSource.IsNull())
            return 1;
    }

    {
        BonePtr src = MakeBone("source", 1);
        BonePtr dest = MakeBone("dest", 1);
        if (builder.AssignAnimation(*dest, src, true) != 1 || dest->AnimSource.Get() != src.Get()
            || dest->ExtensionParent)
            return 2;
        BonePtr normal = MakeBone("normal");
        dest->Children.push_back(normal);
        if (builder.AssignAnimation(*dest, src, true) != 1 || !normal->AnimSource.IsNull())
            return 3;
        BonePtr sourceChild = MakeBone("child");
        src->Children.push_back(sourceChild);
        if (builder.AssignAnimation(*dest, src, true) != 1 || normal->AnimSource.Get() != sourceChild.Get())
            return 4;
    }

    {
        BonePtr src = MakeBone("source", 1);
        BonePtr srcArm = MakeBone("arm");
        src->Children.push_back(srcArm);
        BonePtr dest = MakeBone("dest", 1);
        BonePtr prefix = MakeBone("ADDED_BONE", 2);
        dest->Children.push_back(prefix);
        if (builder.AssignAnimation(*dest, src, true) != 1 || prefix->AnimSource.Get() != srcArm.Get()
            || dest->ExtensionParent || prefix->ExtensionChild)
            return 5;

        BonePtr destArm = MakeBone("arm");
        BonePtr extra = MakeBone("unused");
        prefix->Children.push_back(destArm);
        prefix->Children.push_back(extra);
        if (builder.AssignAnimation(*dest, src, true) != 2 || !dest->ExtensionParent || !prefix->ExtensionChild
            || !prefix->AnimSource.IsNull() || destArm->AnimSource.Get() != srcArm.Get() || !extra->AnimSource.IsNull())
            return 6;
    }
    puts("00ab1740_TEST PASS");
    return 0;
}