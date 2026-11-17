// CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn(CMemoryDataInputStream& is, CHeroStatIncreaseData& value) -- read the entry count, then per entry the stat category and its float increase (three inline stream reads) and insert the pair into StatIncreases
// Flags: /O2 /Oy /W3
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCHeroStatIncreaseData@@@@QAEXAAVCMemoryDataInputStream@@AAVCHeroStatIncreaseData@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DC070, 307 bytes (catalog; bsim .92).
// Identity: Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@VCHeroStatIncreaseData@@@@QAEXAAVCMemoryDataInputStream@@AAVCHeroStatIncreaseData@@@Z
//   (the PDB form takes the value by reference); Ego_r CHeroStatIncreaseData = { map<EHeroExperienceStatCategory, float> StatIncreases }
//   at +0 (engine/CHeroStatIncreaseData.h).
// NF61 (round 7, q10579): moved onto the shared headers (engine/CPersistTraits.h member specialisation, engine/CMemoryDataInputStream.h
//   retail stream with the inline Read / ReadSLONG / ReadFloat, engine/CHeroStatIncreaseData.h); the local CMemoryDataInputStream,
//   CHeroStatIncreaseData and map / insert-result models are gone. The insert now binds NF58's _Rb_tree::insert_unique 0x006DB610;
//   the self leaf is the PDB spelling (was the pointer form `PAVCHeroStatIncreaseData`).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CHeroStatIncreaseData.h"
#include "engine/CPersistTraits.h"

template<> void CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn(CMemoryDataInputStream& is, CHeroStatIncreaseData& value)
{
    unsigned long count = is.ReadULONG();
    while (count != 0) {
        EHeroExperienceStatCategory stat = (EHeroExperienceStatCategory)is.ReadSLONG();
        float increase = is.ReadFloat();
        value.StatIncreases.insert(std::map<EHeroExperienceStatCategory, float>::value_type(stat, increase));
        --count;
    }
}
