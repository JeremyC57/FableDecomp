// 0088b0c0 GFLoadSkeletalMorphTargetFromFile
// Flags: /Gr
// Leaf: ?GFLoadSkeletalMorphTargetFromFile@@YIXAAVCAFile@@AAVC3DSkeletalMorphTarget@@@Z
#define FABLE_RETAIL_SKELETAL_MORPH_FILE_PASS30
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY
#include "engine/CStringParser.h"
#include "engine/CAFile.h"
#include "engine/C3DSkeletalMorph.h"
#include "fable_assert.h"

void FABLE_FASTCALL GFLoadSkeletalMorphTargetFromFile(CAFile& file, C3DSkeletalMorphTarget& skeletal_morph_target)
{
    std::vector<char> bone_data(file.GetLength() + 1);
    file.SetPosition(0);
    file.Read(&bone_data[0], file.GetLength(), 0);
    bone_data[file.GetLength()] = 0;

    CStringParser parser(&bone_data[0]);
    if (!parser.SkipPastString("#Start_Bone_data\r\n")) {
        FABLE_RETAIL_ASSERT(!"Invalid bone file.");
        return;
    }
    CArray<C3DSkeletalBoneMorph> morph_list;
    while (true) {
        if (parser.PeekNextItemType() == PARSED_ITEM_SYMBOL)
            break;

        CCharString bone_name = parser.ReadAsStringUntilString(": ");
        if (!parser.SkipPastNextItemIfIs(": ")) {
            FABLE_RETAIL_ASSERT(!"Invalid bone file.");
            return;
        }
        C3DVector scaling;
        scaling.X = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(", ")) {
            FABLE_RETAIL_ASSERT(!"Invalid bone file.");
            return;
        }
        scaling.Y = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(", ")) {
            FABLE_RETAIL_ASSERT(!"Invalid bone file.");
            return;
        }
        scaling.Z = parser.ReadAsFloat();
        if (!parser.SkipPastNextItemIfIs(";\r\n")) {
            FABLE_RETAIL_ASSERT(!"Invalid bone file.");
            return;
        }
        morph_list.push_back(C3DSkeletalBoneMorph(bone_name, scaling));
    }
    if (!parser.SkipPastNextItemIfIs("#End_bone_data\r\n")) {
        FABLE_RETAIL_ASSERT(!"Invalid bone file.");
        return;
    }
    CWideString name;
    CAFile::SplitPath(file.GetPathname(), 0, 0, &name, 0);
    skeletal_morph_target.Initialise(name.ToCharString(), morph_list);
}