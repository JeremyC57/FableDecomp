// C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& chunk, CSimpleMemoryPool& pool) -- build one bone's compressed animation track from the file's per-frame matrices: name, rotation / position / scaling track modes, quantised and paletted tracks copied into the pool
// Grade: DIFFER(3981v3491, md=1535) -- first draft: rotation / position / scaling palette builders and the pool copy are modelled from the retail control flow; vector locals, the find loops and the inline GFMax / fabs chains are not yet in retail's shape (ours is 490 bytes longer)
// Next idea: match the keyframe and factor loops first (0x00A4F087..0x00A4F28B), then the three palette loops one at a time against retail's register use; check STLport 4.6 find vs the hand loop
// Leaf: ?InitialiseFromFileData@C3DAnimationSequenceData@@QAE_NABVC3DAnimFileSequenceChunk@@AAVCSimpleMemoryPool@@@Z
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CCHARSTRING
#include <string.h>
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CMatrix3x4.h"

inline float __fastcall GFFloatAbs(float f)
{
    *(unsigned long*)&f &= 0x7fffffff;
    return f;
}

template <class T> inline const T& GFMax(const T& a, const T& b) { return (a > b) ? a : b; }

class CQuaternion {
public:
    float X, Y, Z, W;
    CQuaternion() { }
    bool Equals(const CQuaternion& other) const;                                       // 0x00A88C50
    bool IsIdentity() const;                                                           // 0x00A88CB0
};

class C4DVector { public: float X, Y, Z, W; };

class C3DKeyframe {
public:
    CQuaternion Rotation;                                                              // +0x00
    C4DVector   Position;                                                              // +0x10
    C4DVector   Scaling;                                                               // +0x20
    C3DKeyframe() { }
    bool InitialiseFromMatrix(const CMatrix3x4& matrix);                               // 0x00AAF1E0
};

class C3DAnimFileSequenceChunk {
public:
    unsigned char           _pad_0x00[0x34];
    CCharString             BoneName;                                                  // +0x34
    float                   SamplesPerSecond;                                          // +0x38
    unsigned char           BoneType;                                                  // +0x3c
    std::vector<CMatrix3x4> Matrices;                                                  // +0x40
};

class C3DAnimationSequenceData {
public:
    class C3DVectorWord {
    public:
        short X, Y, Z;
        C3DVectorWord() { }
        C3DVectorWord(short x, short y, short z) : X(x), Y(y), Z(z) { }
        bool operator==(const C3DVectorWord& other) const { return X == other.X && Y == other.Y && Z == other.Z; }
    };
    bool InitialiseFromFileData(const C3DAnimFileSequenceChunk& chunk, CSimpleMemoryPool& pool);
private:
    char*           BoneName;                                                          // +0x00
    float           SamplesPerSecond;                                                  // +0x04
    unsigned short  FrameCount;                                                        // +0x08
    unsigned char   BoneType : 5;                                                      // +0x0a
    unsigned char   EnabledFlag : 1;
    unsigned char   AnimatedFlag : 1;
    unsigned char   IdentityFlag : 1;
    unsigned char   RotationTrackMode : 2;                                             // +0x0b
    unsigned char   PositionTrackMode : 2;
    unsigned char   ScalingTrackMode : 2;
    float           PositionFactor;                                                    // +0x0c
    float           ScalingFactor;                                                     // +0x10
    CQuaternion*    RotationTrack;                                                     // +0x14
    unsigned short  RotationTrackSize;                                                 // +0x18
    unsigned short  PositionTrackSize;                                                 // +0x1a
    unsigned short  PalettedRotationTrackSize;                                         // +0x1c
    unsigned short  PalettedPositionTrackSize;                                         // +0x1e
    C3DVectorWord*  PositionTrack;                                                     // +0x20
    unsigned char*  PalettedRotationTrack;                                             // +0x24
    unsigned char*  PalettedPositionTrack;                                             // +0x28
};

bool C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& chunk, CSimpleMemoryPool& pool)
{
    CCharString name(chunk.BoneName);
    BoneName = pool.AllocateType<char>(name.GetLength() + 1);
    strcpy(BoneName, name.GetAsCharArray());
    BoneType = chunk.BoneType;
    SamplesPerSecond = chunk.SamplesPerSecond;
    EnabledFlag = false;
    FrameCount = (unsigned short)chunk.Matrices.size();

    std::vector<C3DKeyframe> keyframes(FrameCount);
    for (unsigned long frame = 0; frame < FrameCount; ++frame) {
        keyframes[frame].InitialiseFromMatrix(chunk.Matrices[frame]);
        keyframes[frame].Scaling.X = 1.0f;
        keyframes[frame].Scaling.Y = 1.0f;
        keyframes[frame].Scaling.Z = 1.0f;
        keyframes[frame].Scaling.W = 0.0f;
    }

    PositionFactor = 0.0f;
    ScalingFactor = 0.0f;
    for (unsigned long i = 0; i < FrameCount; ++i) {
        const C4DVector& position = keyframes[i].Position;
        float largest = GFMax(GFMax(GFFloatAbs(position.X), GFFloatAbs(position.Y)), GFFloatAbs(position.Z));
        PositionFactor = GFMax(PositionFactor, largest);
        const C4DVector& scaling = keyframes[i].Scaling;
        largest = GFMax(GFMax(GFFloatAbs(scaling.X), GFFloatAbs(scaling.Y)), GFFloatAbs(scaling.Z));
        ScalingFactor = GFMax(ScalingFactor, largest);
    }
    PositionFactor *= 1.0f / 32767.0f;
    ScalingFactor *= 1.0f / 32767.0f;
    float position_scale = PositionFactor > 0.0f ? 1.0f / PositionFactor : 0.0f;
    float scaling_scale = ScalingFactor > 0.0f ? 1.0f / ScalingFactor : 0.0f;

    std::vector<CQuaternion> rotations;
    std::vector<CQuaternion> unique_rotations;
    std::vector<unsigned char> rotation_palette;
    std::vector<C3DVectorWord> positions;
    std::vector<C3DVectorWord> unique_positions;
    std::vector<unsigned char> position_palette;
    std::vector<C3DVectorWord> scalings;
    std::vector<C3DVectorWord> unique_scalings;
    std::vector<unsigned char> scaling_palette;

    rotations.resize(FrameCount);
    for (unsigned long r = 0; r < FrameCount; ++r) {
        const CQuaternion& rotation = keyframes[r].Rotation;
        rotations[r] = rotation;
        unsigned long count = unique_rotations.size();
        unsigned long index = 0;
        for (; index < count; ++index)
            if (rotation.Equals(unique_rotations[index]))
                break;
        if (index == count)
            unique_rotations.push_back(rotation);
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
        if (unique_rotations.size() * sizeof(CQuaternion) + rotation_palette.size() < rotations.size() * sizeof(CQuaternion)
            && unique_rotations.size() <= 255) {
            RotationTrackMode = 3;
            rotations = unique_rotations;
        } else {
            RotationTrackMode = 2;
            rotation_palette.clear();
        }
    }

    positions.resize(FrameCount);
    position_palette.resize(FrameCount, 0);
    for (unsigned long p = 0; p < FrameCount; ++p) {
        const C4DVector& position = keyframes[p].Position;
        C3DVectorWord word((short)(position.X * position_scale), (short)(position.Y * position_scale),
                           (short)(position.Z * position_scale));
        positions[p] = word;
        unsigned long count = unique_positions.size();
        unsigned long index = 0;
        for (; index < count; ++index)
            if (word == unique_positions[index])
                break;
        if (index == count)
            unique_positions.push_back(word);
        position_palette[p] = (unsigned char)index;
    }
    if (unique_positions.size() == 1) {
        const C4DVector& position = keyframes[0].Position;
        if (position.X * position.X + position.Y * position.Y + position.Z * position.Z <= 0.0001f * 0.0001f) {
            PositionTrackMode = 0;
            positions.clear();
        } else {
            PositionTrackMode = 1;
            positions = unique_positions;
        }
    } else {
        AnimatedFlag = true;
        if (unique_positions.size() * sizeof(C3DVectorWord) + position_palette.size() < positions.size() * sizeof(C3DVectorWord)
            && unique_positions.size() <= 255) {
            PositionTrackMode = 3;
            positions = unique_positions;
        } else {
            PositionTrackMode = 2;
            position_palette.clear();
        }
    }

    scalings.resize(FrameCount);
    scaling_palette.resize(FrameCount, 0);
    for (unsigned long s = 0; s < FrameCount; ++s) {
        const C4DVector& scaling = keyframes[s].Scaling;
        C3DVectorWord word((short)(scaling.X * scaling_scale), (short)(scaling.Y * scaling_scale),
                           (short)(scaling.Z * scaling_scale));
        scalings[s] = word;
        unsigned long count = unique_scalings.size();
        unsigned long index = 0;
        for (; index < count; ++index)
            if (word == unique_scalings[index])
                break;
        if (index == count)
            unique_scalings.push_back(word);
        scaling_palette[s] = (unsigned char)index;
    }
    if (unique_scalings.size() == 1) {
        const C4DVector& scaling = keyframes[0].Scaling;
        float x = scaling.X - 1.0f, y = scaling.Y - 1.0f, z = scaling.Z - 1.0f;
        if (x * x + y * y + z * z <= 0.0001f * 0.0001f)
            ScalingTrackMode = 0;
        else {
            ScalingTrackMode = 1;
            scalings = unique_scalings;
        }
    } else {
        AnimatedFlag = true;
        if (unique_scalings.size() * sizeof(C3DVectorWord) + scaling_palette.size() < scalings.size() * sizeof(C3DVectorWord)
            && unique_scalings.size() <= 255) {
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
