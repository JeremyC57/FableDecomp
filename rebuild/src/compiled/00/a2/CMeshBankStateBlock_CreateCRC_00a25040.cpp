// 00a25040
// Leaf: ?CreateCRC@CMeshBankStateBlock@@UAEKXZ
// Flags: none
#define FABLE_RETAIL_CRC32TAB_PASS29
#define FABLE_RETAIL_PASS22_CCHARSTRING_GETASCHARARRAY_ORIGINAL_IFELSE
#define FABLE_RETAIL_CCRC_ORIGINAL_TYPED_LOOP_PASS27
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#define FABLE_RETAIL_MESHBANK_STATE_CALCCRC_PASS29
#define FABLE_RETAIL_CMESHBANKSTATEBLOCK
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCHARSTRING_GETCRC_BYTELENGTH_PASS27
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_STLP_SET
#include "engine/CCRC.h"
#include "engine/C3DBoundingBox.h"
#include "engine/CMeshBankStateBlock.h"

unsigned long CMeshBankStateBlock::CreateCRC()
{
    unsigned long crc = CalcCRC(&State, sizeof(State));
    CLODStateBlock::TBoneLODSet::iterator bone_lod_itr;
    for (bone_lod_itr = LODState.BoneLODs.begin(); bone_lod_itr != LODState.BoneLODs.end(); ++bone_lod_itr) {
        CCharString bone_name = bone_lod_itr->first;
        crc = CCRC::Calc(crc, bone_name.GetAsCharArray(), bone_name.GetByteLength());
        long lod = bone_lod_itr->second;
        crc = CCRC::Calc(crc, &lod, sizeof(lod));
    }
    CAnimationStateBlock::TBoneSet::iterator bone_itr;
    for (bone_itr = AnimationState.ExcludedBones.begin(); bone_itr != AnimationState.ExcludedBones.end(); ++bone_itr) {
        CCharString bone_name = *bone_itr;
        crc = CCRC::Calc(crc, bone_name.GetAsCharArray(), bone_name.GetByteLength());
    }
    if (LODState.MinXYZRange.Get()) {
        crc = CCRC::Calc(crc, &LODState.MinXYZRange->PeekMinCoord(), sizeof(C3DVector));
        crc = CCRC::Calc(crc, &LODState.MinXYZRange->PeekMaxCoord(), sizeof(C3DVector));
    }
    return crc;
}