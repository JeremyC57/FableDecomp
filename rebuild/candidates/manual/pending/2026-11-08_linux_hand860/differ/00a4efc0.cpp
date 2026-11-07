// C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& chunk, CSimpleMemoryPool& pool) -- build one bone's compressed animation track from the file's per-frame matrices: name, rotation / position / scaling track modes, quantised and paletted tracks copied into the pool
// Grade: DIFFER(3495v3491, md=1268) -- length now within 4 bytes: positions / scalings and their palettes declared at their resize, which folds each resize to retail's `push &x; push FrameCount; push 0; call _M_fill_insert` (0x00A552B0 / 0x00485F3A); residue: retail keeps 0 in ebx from the entry (`xor ebx,ebx`, four callee-saved pushes v our two), the keyframe loop's single induction pointer, the factor loop's x87 order (retail fcomp st(1) GFMax chain), and the frame-slot order those move
// Next idea: retail declares the track vectors at the top yet still folds the resizes, so VC7.1 knew they were untouched: try the same-TU visible-callee lever (STLport vector members and CQuaternion::Equals / IsIdentity / C3DKeyframe::InitialiseFromMatrix bodies under auto_inline(off)) with the declarations back at the top; then the entry `xor ebx,ebx` zero register
// Leaf: ?InitialiseFromFileData@C3DAnimationSequenceData@@QAE_NABVC3DAnimFileSequenceChunk@@AAVCSimpleMemoryPool@@@Z
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_VECTORWORD
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_MATRICES
#include <string.h>
#include <math.h>
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/C3DAnimationSequenceData.h"
#include "engine/C3DAnimFileSequenceChunk.h"

template <class T> inline T GFMax(const T& a, const T& b) { return (a > b) ? a : b; }

// Out-of-line in retail (0x00A55070, landed): the C3DVectorWord growth path is called from both push_back sites
// (0x00A4F71F, 0x00A4F999) while the CQuaternion one is expanded (0x00A4F3D1..0x00A4F48A): a declaration of the instantiation.
namespace std {
template <> void vector<C3DAnimationSequenceData::C3DVectorWord, allocator<C3DAnimationSequenceData::C3DVectorWord> >::_M_insert_overflow(
    C3DAnimationSequenceData::C3DVectorWord* position, const C3DAnimationSequenceData::C3DVectorWord& x, const __false_type&,
    size_type fillLength, bool atEnd);
}

// CQuaternion (Ego_r.pdb; no shared header): X, Y, Z, W.
class CQuaternion {
public:
    float X, Y, Z, W;
    CQuaternion() { }
    bool Equals(const CQuaternion& other) const;                                       // 0x00A88C50
    bool IsIdentity() const;                                                           // 0x00A88CB0
};

// C3DKeyframe (Ego_r.pdb; no shared header): Rotation +0x00, Position +0x10, Scaling +0x20.
class C3DKeyframe {
public:
    CQuaternion Rotation;                                                              // +0x00
    C4DVector   Position;                                                              // +0x10
    C4DVector   Scaling;                                                               // +0x20
    C3DKeyframe() { }
    bool InitialiseFromMatrix(const CMatrix3x4& matrix);                               // 0x00AAF1E0
};

bool C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& chunk, CSimpleMemoryPool& pool)
{
    CCharString name(chunk.BoneName);
    BoneName = pool.AllocateType<char>(name.GetLength() + 1);
    strcpy(BoneName, name.GetAsCharArray());
    BoneType = chunk.BoneType;
    SamplesPerSecond = chunk.SamplesPerSecond;
    AnimatedFlag = false;
    FrameCount = (unsigned short)chunk.Matrices.size();

    CArray<C3DKeyframe> keyframes(FrameCount);
    C3DKeyframe* keyframe_data = keyframes.begin();
    for (unsigned long frame = 0; frame < FrameCount; ++frame) {
        C3DKeyframe& keyframe = keyframe_data[frame];
        keyframe.InitialiseFromMatrix(chunk.Matrices[frame]);
        keyframe.Scaling.X = 1.0f;
        keyframe.Scaling.Y = 1.0f;
        keyframe.Scaling.Z = 1.0f;
        keyframe.Scaling.W = 0.0f;
    }

    PositionFactor = 0.0f;
    ScalingFactor = 0.0f;
    for (unsigned long i = 0; i < FrameCount; ++i) {
        C3DVector position = keyframes[i].Position.ToC3DVector();
        float largest = GFMax(GFMax((float)fabs(position.X), (float)fabs(position.Y)), (float)fabs(position.Z));
        PositionFactor = GFMax(PositionFactor, largest);
        C3DVector scaling = keyframes[i].Scaling.ToC3DVector();
        largest = GFMax(GFMax((float)fabs(scaling.X), (float)fabs(scaling.Y)), (float)fabs(scaling.Z));
        ScalingFactor = GFMax(ScalingFactor, largest);
    }
    PositionFactor *= 1.0f / 32767.0f;
    ScalingFactor *= 1.0f / 32767.0f;
    float position_scale = 0.0f;
    if (PositionFactor > 0.0f)
        position_scale = 1.0f / PositionFactor;
    float scaling_scale = 0.0f;
    if (ScalingFactor > 0.0f)
        scaling_scale = 1.0f / ScalingFactor;

    CArray<CQuaternion> rotations;
    CArray<unsigned char> rotation_palette;

    rotations.resize(FrameCount);
    CArray<CQuaternion> unique_rotations;
    for (unsigned long r = 0; r < FrameCount; ++r) {
        const CQuaternion& rotation = keyframes[r].Rotation;
        rotations[r] = rotation;
        unsigned long count = unique_rotations.size();
        long index = -1;
        for (unsigned long u = 0; u < count; ++u) {
            if (rotation.Equals(unique_rotations[u])) {
                index = u;
                break;
            }
        }
        if (index == -1) {
            index = count;
            unique_rotations.push_back(rotation);
        }
        rotation_palette.push_back((unsigned char)index);
    }
    if (unique_rotations.size() == 1) {
        if (unique_rotations[0].IsIdentity()) {
            RotationTrackMode = 0;
            rotations.clear();
        } else {
            RotationTrackMode = 1;
            rotations = unique_rotations;
        }
        rotation_palette.clear();
    } else {
        AnimatedFlag = true;
        if (unique_rotations.GetByteSize() + rotation_palette.GetByteSize() < rotations.GetByteSize() && unique_rotations.size() <= 255) {
            RotationTrackMode = 3;
            rotations = unique_rotations;
        } else {
            RotationTrackMode = 2;
            rotation_palette.clear();
        }
    }

    CArray<C3DVectorWord> positions;
    CArray<unsigned char> position_palette;
    positions.resize(FrameCount);
    position_palette.resize(FrameCount, 0);
    CArray<C3DVectorWord> unique_positions;
    for (unsigned long p = 0; p < FrameCount; ++p) {
        positions[p] = C3DVectorWord(keyframes[p].Position.ToC3DVector() * position_scale);
        const C3DVectorWord& word = positions[p];
        unsigned long count = unique_positions.size();
        long index = -1;
        for (unsigned long u = 0; u < count; ++u) {
            if (word == unique_positions[u]) {
                index = u;
                break;
            }
        }
        if (index == -1) {
            index = count;
            unique_positions.push_back(word);
        }
        position_palette[p] = (unsigned char)index;
    }
    if (unique_positions.size() == 1) {
        if (!keyframes[0].Position.ToC3DVector().IsNonZero()) {
            PositionTrackMode = 0;
            positions.clear();
        } else {
            PositionTrackMode = 1;
            positions = unique_positions;
        }
        position_palette.clear();
    } else {
        AnimatedFlag = true;
        if (unique_positions.GetByteSize() + position_palette.GetByteSize() < positions.GetByteSize() && unique_positions.size() <= 255) {
            PositionTrackMode = 3;
            positions = unique_positions;
        } else {
            PositionTrackMode = 2;
            position_palette.clear();
        }
    }

    CArray<C3DVectorWord> scalings;
    CArray<unsigned char> scaling_palette;
    scalings.resize(FrameCount);
    scaling_palette.resize(FrameCount, 0);
    CArray<C3DVectorWord> unique_scalings;
    for (unsigned long s = 0; s < FrameCount; ++s) {
        scalings[s] = C3DVectorWord(keyframes[s].Scaling.ToC3DVector() * scaling_scale);
        const C3DVectorWord& word = scalings[s];
        unsigned long count = unique_scalings.size();
        long index = -1;
        for (unsigned long u = 0; u < count; ++u) {
            if (word == unique_scalings[u]) {
                index = u;
                break;
            }
        }
        if (index == -1) {
            index = count;
            unique_scalings.push_back(word);
        }
        scaling_palette[s] = (unsigned char)index;
    }
    if (unique_scalings.size() == 1) {
        if ((keyframes[0].Scaling.ToC3DVector() - C3DVector(1.0f, 1.0f, 1.0f)).GetMagnitude() < DELTA_NOTIONAL_ZERO)
            ScalingTrackMode = 0;
        else {
            ScalingTrackMode = 1;
            scalings = unique_scalings;
        }
    } else {
        AnimatedFlag = true;
        if (unique_scalings.GetByteSize() + scaling_palette.GetByteSize() < scalings.GetByteSize() && unique_scalings.size() <= 255) {
            ScalingTrackMode = 3;
            scalings = unique_scalings;
        } else
            ScalingTrackMode = 2;
    }

    RotationTrackSize = (unsigned short)rotations.size();
    PositionTrackSize = (unsigned short)positions.size();
    PalettedRotationTrackSize = (unsigned short)rotation_palette.size();
    PalettedPositionTrackSize = (unsigned short)position_palette.size();
    RotationTrack = pool.AllocateType<CQuaternion>(RotationTrackSize);
    PositionTrack = pool.AllocateType<C3DVectorWord>(PositionTrackSize);
    PalettedRotationTrack = pool.AllocateType<unsigned char>(PalettedRotationTrackSize);
    PalettedPositionTrack = pool.AllocateType<unsigned char>(PalettedPositionTrackSize);
    CQuaternion* rotation_out = RotationTrack;
    const CQuaternion* rotation_in = rotations.begin();
    for (long rn = rotations.end() - rotations.begin(); rn > 0; --rn)
        *rotation_out++ = *rotation_in++;
    C3DVectorWord* position_out = PositionTrack;
    const C3DVectorWord* position_in = positions.begin();
    for (long pn = positions.end() - positions.begin(); pn > 0; --pn)
        *position_out++ = *position_in++;
    if (rotation_palette.begin() != rotation_palette.end())
        memmove(PalettedRotationTrack, rotation_palette.begin(), rotation_palette.end() - rotation_palette.begin());
    if (position_palette.begin() != position_palette.end())
        memmove(PalettedPositionTrack, position_palette.begin(), position_palette.end() - position_palette.begin());
    IdentityFlag = (RotationTrackMode == 0 && PositionTrackMode == 0 && ScalingTrackMode == 0);
    return true;
}
