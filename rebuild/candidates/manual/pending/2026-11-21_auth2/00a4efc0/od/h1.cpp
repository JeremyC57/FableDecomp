// C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& file_data, CSimpleMemoryPool& alloc) -- compress one bone's file matrices into identity / constant / normal / paletted tracks
// Leaf: ?InitialiseFromFileData@C3DAnimationSequenceData@@QAE_NABVC3DAnimFileSequenceChunk@@AAVCSimpleMemoryPool@@@Z
// Flags: /Gr
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_COPY
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD
#define FABLE_ALLOCATETYPE_ASSERT_COND false
#define FABLE_AT_V 1
#include "fable_assert.h"
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX

#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_GETTERS
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_FROMFILEDATA
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_VECTORWORD_FROMVECTOR
#define FABLE_RETAIL_C3DKEYFRAME_FROMMATRIX
#define FABLE_RETAIL_C3DKEYFRAME_GETROTATION
#define FABLE_RETAIL_CQUATERNION_EQUALS
#include <string.h>
#include <math.h>
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CQuaternion.h"
#include "engine/C3DKeyframe.h"
#include "engine/CChunkedFileChunk.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/C3DAnimFileSequenceChunk.h"
#include "engine/C3DAnimationSequenceData.h"

bool C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& file_data, CSimpleMemoryPool& alloc)
{
    CCharString bone_name = file_data.GetBoneName();
    BoneType = file_data.GetBoneType();
    SamplesPerSecond = file_data.GetSamplesPerSecond();
    FrameCount = (unsigned short)file_data.GetMatrixCount();
    const CMatrix3x4& matrix = *file_data.GetMatrix(1);
    CArray<C3DKeyframe> keyframes(FrameCount);
    C3DKeyframe& keyframe = keyframes[1];
    keyframe.SetScaling(C3DVector(1, 1, 1));
    PositionFactor = GetMaximumAbsoluteComponent(keyframe.GetPosition());
    ScalingFactor = GetMaximumAbsoluteComponent(keyframe.GetScaling());
    CQuaternion q = keyframe.GetRotation();
    C3DVectorWord w(keyframe.GetPosition());
    C3DVectorWord w2;
    bool eq = w == w2;
    CArray<CQuaternion> rotation_track;
    unsigned long s = rotation_track.GetByteSize();
    BoneName = alloc.AllocateType<char>(3);
    RotationTrack = alloc.AllocateType<CQuaternion>(3);
    SetupIdentityFlag();
    return eq && s && &matrix;
}
void C3DAnimationSequenceData::SetupIdentityFlag()
{
    if (RotationTrackMode == TRACKMODE_IDENTITY && PositionTrackMode == TRACKMODE_IDENTITY && ScalingTrackMode == TRACKMODE_IDENTITY)
        IdentityFlag = true;
    else
        IdentityFlag = false;
}

float C3DAnimationSequenceData::GetMaximumAbsoluteComponent(const C3DVector& vect)
{
    return ((float)fabs(vect.X) > (float)fabs(vect.Y) ? (float)fabs(vect.X) : (float)fabs(vect.Y)) > (float)fabs(vect.Z) ?
        ((float)fabs(vect.X) > (float)fabs(vect.Y) ? (float)fabs(vect.X) : (float)fabs(vect.Y)) : (float)fabs(vect.Z);
}
