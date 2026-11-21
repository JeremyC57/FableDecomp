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
    BoneName = alloc.AllocateType<char>(bone_name.GetLength() + 1);
    strcpy(BoneName, bone_name.GetAsCharArray());
    BoneType = file_data.GetBoneType();
    SamplesPerSecond = file_data.GetSamplesPerSecond();
    FrameCount = (unsigned short)file_data.GetMatrixCount();
    AnimatedFlag = false;
    bool status = true;
    CArray<C3DKeyframe> keyframes(FrameCount);
    for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const CMatrix3x4& matrix = *file_data.GetMatrix(c_frame);
        C3DKeyframe& keyframe = keyframes[c_frame];
        keyframe.InitialiseFromMatrix(matrix);
        keyframe.SetScaling(C3DVector(1, 1, 1));
    }
    PositionFactor = 0.0f;
    ScalingFactor = 0.0f;
    for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const C3DKeyframe& keyframe = keyframes[c_frame];
        PositionFactor = (std::max)(PositionFactor, GetMaximumAbsoluteComponent(keyframe.GetPosition()));
        ScalingFactor = (std::max)(ScalingFactor, GetMaximumAbsoluteComponent(keyframe.GetScaling()));
    }
    PositionFactor *= 1.0f / 32767.0f;
    ScalingFactor *= 1.0f / 32767.0f;
    float position_normalise = PositionFactor > 0.0f ? 1.0f / PositionFactor : 0.0f;
    float scaling_normalise = ScalingFactor > 0.0f ? 1.0f / ScalingFactor : 0.0f;
    CArray<CQuaternion> rotation_track;
    CArray<C3DVectorWord> position_track, scaling_track;
    CArray<unsigned char> paletted_rotation_track, paletted_position_track, paletted_scaling_track;
    rotation_track.resize(FrameCount);
    paletted_rotation_track.clear();
    CArray<CQuaternion> unique_rotations;
    for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const C3DKeyframe& keyframe = keyframes[c_frame];
        rotation_track[c_frame] = keyframe.GetRotation();
        long unique_index = -1;
        for (unsigned long c_unique = 0; c_unique < unique_rotations.size(); ++c_unique) {
            if (keyframe.Rotation.Equals(unique_rotations[c_unique])) {
                unique_index = c_unique;
                break;
            }
        }
        if (unique_index == -1) {
            unique_index = unique_rotations.size();
            unique_rotations.push_back(keyframe.Rotation);
        }
        paletted_rotation_track.push_back((unsigned char)unique_index);
    }
    if (unique_rotations.size() == 1) {
        if (unique_rotations[0].IsIdentity()) {
            RotationTrackMode = TRACKMODE_IDENTITY;
            rotation_track.clear();
            paletted_rotation_track.clear();
        } else {
            RotationTrackMode = TRACKMODE_CONSTANT;
            rotation_track = unique_rotations;
            paletted_rotation_track.clear();
        }
    } else {
        AnimatedFlag = true;
        unsigned long compressed_size = unique_rotations.GetByteSize() + paletted_rotation_track.GetByteSize();
        unsigned long uncompressed_size = rotation_track.GetByteSize();
        if (compressed_size < uncompressed_size && unique_rotations.size() <= 255) {
            RotationTrackMode = TRACKMODE_PALETTED;
            rotation_track = unique_rotations;
        } else {
            RotationTrackMode = TRACKMODE_NORMAL;
            paletted_rotation_track.clear();
        }
    }
    position_track.resize(FrameCount);
    paletted_position_track.resize(FrameCount);
    CArray<C3DVectorWord> unique_positions;
    for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const C3DKeyframe& keyframe = keyframes[c_frame];
        C3DVectorWord& position = position_track[c_frame];
        position = keyframe.GetPosition() * position_normalise;
        long unique_index = -1;
        for (unsigned long c_unique = 0; c_unique < unique_positions.size(); ++c_unique) {
            if (position == unique_positions[c_unique]) {
                unique_index = c_unique;
                break;
            }
        }
        if (unique_index == -1) {
            unique_index = unique_positions.size();
            unique_positions.push_back(position);
        }
        paletted_position_track[c_frame] = (unsigned char)unique_index;
    }
    if (unique_positions.size() == 1) {
        const C3DKeyframe& keyframe = keyframes[0];
        if (!keyframe.GetPosition().IsNonZero()) {
            PositionTrackMode = TRACKMODE_IDENTITY;
            position_track.clear();
        } else {
            PositionTrackMode = TRACKMODE_CONSTANT;
            position_track = unique_positions;
        }
        paletted_position_track.clear();
    } else {
        AnimatedFlag = true;
        unsigned long compressed_size = unique_positions.GetByteSize() + paletted_position_track.GetByteSize();
        unsigned long uncompressed_size = position_track.GetByteSize();
        if (compressed_size < uncompressed_size && unique_positions.size() <= 255) {
            PositionTrackMode = TRACKMODE_PALETTED;
            position_track = unique_positions;
        } else {
            PositionTrackMode = TRACKMODE_NORMAL;
            paletted_position_track.clear();
        }
    }
    scaling_track.resize(FrameCount);
    paletted_scaling_track.resize(FrameCount);
    CArray<C3DVectorWord> unique_scalings;
    for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const C3DKeyframe& keyframe = keyframes[c_frame];
        C3DVectorWord& scaling = scaling_track[c_frame];
        scaling = keyframe.GetScaling() * scaling_normalise;
        long unique_index = -1;
        for (unsigned long c_unique = 0; c_unique < unique_scalings.size(); ++c_unique) {
            if (scaling == unique_scalings[c_unique]) {
                unique_index = c_unique;
                break;
            }
        }
        if (unique_index == -1) {
            unique_index = unique_scalings.size();
            unique_scalings.push_back(scaling);
        }
        paletted_scaling_track[c_frame] = (unsigned char)unique_index;
    }
    if (unique_scalings.size() == 1) {
        const C3DKeyframe& keyframe = keyframes[0];
        if ((keyframe.GetScaling() - C3DVector(1, 1, 1)).GetMagnitude() < DELTA_NOTIONAL_ZERO) {
            ScalingTrackMode = TRACKMODE_IDENTITY;
            scaling_track.clear();
        } else {
            ScalingTrackMode = TRACKMODE_CONSTANT;
            scaling_track = unique_scalings;
        }
        paletted_scaling_track.clear();
    } else {
        AnimatedFlag = true;
        unsigned long compressed_size = unique_scalings.GetByteSize() + paletted_scaling_track.GetByteSize();
        unsigned long uncompressed_size = scaling_track.GetByteSize();
        if (compressed_size < uncompressed_size && unique_scalings.size() <= 255) {
            ScalingTrackMode = TRACKMODE_PALETTED;
            scaling_track = unique_scalings;
        } else {
            ScalingTrackMode = TRACKMODE_NORMAL;
            paletted_scaling_track.clear();
        }
    }
    RotationTrackSize = (unsigned short)rotation_track.size();
    PositionTrackSize = (unsigned short)position_track.size();
    PalettedRotationTrackSize = (unsigned short)paletted_rotation_track.size();
    PalettedPositionTrackSize = (unsigned short)paletted_position_track.size();
    RotationTrack = alloc.AllocateType<CQuaternion>(RotationTrackSize);
    PositionTrack = alloc.AllocateType<C3DVectorWord>(PositionTrackSize);
    PalettedRotationTrack = alloc.AllocateType<unsigned char>(PalettedRotationTrackSize);
    PalettedPositionTrack = alloc.AllocateType<unsigned char>(PalettedPositionTrackSize);
    std::copy(rotation_track.begin(), rotation_track.end(), RotationTrack);
    std::copy(position_track.begin(), position_track.end(), PositionTrack);
    std::copy(paletted_rotation_track.begin(), paletted_rotation_track.end(), PalettedRotationTrack);
    std::copy(paletted_position_track.begin(), paletted_position_track.end(), PalettedPositionTrack);
    SetupIdentityFlag();
    return status;
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
