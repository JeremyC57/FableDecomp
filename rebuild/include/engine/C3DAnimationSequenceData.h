#ifndef FABLE_ENGINE_C3DANIMATIONSEQUENCEDATA_H
#define FABLE_ENGINE_C3DANIMATIONSEQUENCEDATA_H
#ifdef FABLE_RETAIL_ANIMATION_BONE_TREE_PASS30
#include "fable_string.h"
#endif
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#include "engine/C3DVector.h"
class C3DKeyframe;
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCE_GETKEYFRAME
class C3DKeyframe;
#endif
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationSequenceData{
public:
 class C3DVectorWord;
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
 // Retail opt-in (chunk 2005, row 0x00A4D920): FableWin.pdb ?InlineGetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@KM@Z
 // (expanded in retail; the includer defines the FABLE_FORCEINLINE body) and Ego_r.pdb ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@KM@Z
 // (retail 0x00A4C1F0, its out-of-line copy). C3DVectorWord: Ego_r.pdb short X/Y/Z (6 bytes) and FableWin.pdb
 // ??DC3DVectorWord@C3DAnimationSequenceData@@QBE?AVC3DVector@@M@Z (scale to a C3DVector; expanded in retail).
 void GetKeyframeInterpolate(C3DKeyframe& keyframe, unsigned long frame, float fraction) const;
 void InlineGetKeyframeInterpolate(C3DKeyframe& keyframe, unsigned long frame, float fraction) const;
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_ISIDENTITY
 // Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): Ego_r.pdb / FableWin.pdb ?IsIdentity@C3DAnimationSequenceData@@QBE_NXZ,
 // inline in retail (the IdentityFlag bit 7 of +0x0A read in place: `mov al,[ecx+0Ah] ; test al,al ; js`, 0x00AA024D).
 bool IsIdentity() const { return IdentityFlag; }
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCE_GETKEYFRAME
 // Retail opt-in (chunk 2014, row 0x00A4E300): Ego_r.pdb / FableWin.pdb ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@M@Z
 // (retail 0x00A4DEA0, out of line, `ret 8`; landed row C3DAnimationSequenceData_GetKeyframeInterpolate_00a4dea0, bound spelling of 0x00A4E250).
 void GetKeyframeInterpolate(C3DKeyframe& keyframe, float time) const;
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCE_INITIALISE
 // Retail opt-in (chunk 2014, row 0x00A50510): Ego_r.pdb ?InitialiseAsConstant@C3DAnimationSequenceData@@QAEXABVCMatrix3x4@@KMAAVCSimpleMemoryPool@@@Z
 // (retail 0x00A4FD70, out of line, `ret 10h`).
 void InitialiseAsConstant(const CMatrix3x4& matrix, unsigned long frame_count, float samples_per_second, CSimpleMemoryPool& memory_pool);
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_READ
 // Retail opt-in (chunk 2033, row 0x00A4C5E0): Ego_r.pdb / FableWin.pdb
 // ?ReadCompressedFromFile@C3DAnimationSequenceData@@QAEXAAVCDataInputStream@@AAVCSimpleMemoryPool@@@Z (retail 0x00A4C5E0, called by the
 // landed C3DAnimationSequence::ReadCompressedFromFile 0x00A4CDD0) and the private ?SetupIdentityFlag@C3DAnimationSequenceData@@AAEXXZ
 // (no out-of-line copy in retail; the reader expands it at 0x00A4CCDF: `test byte ptr [ebp+0Bh],3Fh`, IdentityFlag set / cleared).
 void ReadCompressedFromFile(CDataInputStream& in, CSimpleMemoryPool& pool);
private:
 void SetupIdentityFlag();
public:
#endif
#ifdef FABLE_RETAIL_ANIMATION_BONE_TREE_PASS30
 // FableWin 034870E0 chooses a CCharString temporary, then copy-constructs the result.
 // Inline-budget: the native 00AB0BF0 sequence forwarder expands this complete string selection.
 FABLE_FORCEINLINE CCharString GetBoneName() const { return BoneName ? CCharString(BoneName) : CCharString(""); }
#endif
 C3DAnimationSequenceData():BoneName(0),RotationTrack(0),RotationTrackSize(0),PositionTrackSize(0),PalettedRotationTrackSize(0),PalettedPositionTrackSize(0),PositionTrack(0),PalettedRotationTrack(0),PalettedPositionTrack(0){FrameCount=0;IdentityFlag=false;}
 unsigned long GetFrameCount()const{return FrameCount;}float GetSamplesPerSecond()const{return SamplesPerSecond;}
 bool InitialiseFromFileData(const C3DAnimFileSequenceChunk&,CSimpleMemoryPool&);
private:
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_FROMFILEDATA
 // Retail opt-in (p25C, row 0x00A4EFC0 InitialiseFromFileData): Ego_d.pdb nested enum C3DAnimationSequenceData::ETrackMode
 // (type 0x2901D: TRACKMODE_IDENTITY 0, CONSTANT 1, NORMAL 2, PALETTED 3, MOTION_BIT 2), stored in the 2-bit track-mode fields;
 // and the private helper Ego_r.pdb / FableWin.pdb ?GetMaximumAbsoluteComponent@C3DAnimationSequenceData@@AAEMABVC3DVector@@@Z
 // (no out-of-line copy in retail: expanded twice per frame in the row's factor loop, 0x00A4F110..). Declaration only: the row's
 // TU defines it.
 enum ETrackMode { TRACKMODE_IDENTITY=0, TRACKMODE_CONSTANT=1, TRACKMODE_NORMAL=2, TRACKMODE_PALETTED=3, TRACKMODE_MOTION_BIT=2 };
 float GetMaximumAbsoluteComponent(const C3DVector& vect);
#ifndef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_READ
 // The private ?SetupIdentityFlag@C3DAnimationSequenceData@@AAEXXZ (see READ above; expanded at the end of the row, 0x00A4FC83:
 // `test byte ptr [esi+0Bh],3Fh`). Declaration only, as under READ.
 void SetupIdentityFlag();
#endif
#endif
char* BoneName;float SamplesPerSecond;unsigned short FrameCount;
 unsigned char BoneType:5;unsigned char EnabledFlag:1;unsigned char AnimatedFlag:1;unsigned char IdentityFlag:1;
 unsigned char RotationTrackMode:2;unsigned char PositionTrackMode:2;unsigned char ScalingTrackMode:2;
 float PositionFactor,ScalingFactor;CQuaternion* RotationTrack;
 unsigned short RotationTrackSize,PositionTrackSize,PalettedRotationTrackSize,PalettedPositionTrackSize;
 C3DVectorWord* PositionTrack;unsigned char* PalettedRotationTrack;unsigned char* PalettedPositionTrack;
};
FABLE_STATIC_ASSERT_AT(60, sizeof(C3DAnimationSequenceData)==0x2c);
#if defined(FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_READ) && !defined(FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE)
// (chunk 2033) the reader allocates PositionTrack as an array of C3DVectorWord: Ego_r.pdb short X / Y / Z, 6 bytes (the retail
// AllocateType<C3DVectorWord> size check multiplies by 6 at 0x00A4CB7F).
class C3DAnimationSequenceData::C3DVectorWord {
public:
 short X,Y,Z;
};
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_VECTORWORD_FROMVECTOR
// Retail opt-in (p25C, row 0x00A4EFC0 InitialiseFromFileData): FableWin.pdb ??0C3DVectorWord@C3DAnimationSequenceData@@QAE@XZ,
// ??0C3DVectorWord@C3DAnimationSequenceData@@QAE@ABVC3DVector@@@Z and ??8C3DVectorWord@C3DAnimationSequenceData@@QBE_NABV01@@Z
// (Ego_d.pdb field list 0x29DF1: two constructors, operator==, operator*, X / Y / Z, Sizeof), all inline in retail: the
// conversion is three _ftol2 calls into a stack temporary (0x00A4F654..0x00A4F66C), the comparison three word compares
// (0x00A4F6B0..0x00A4F6CA). The user default constructor makes the element non-POD (STLport's __false_type paths, as the
// landed 0x00A55070 _M_insert_overflow). Needs FABLE_RETAIL_C3DVECTOR (the full C3DVector).
class C3DAnimationSequenceData::C3DVectorWord {
public:
 short X,Y,Z;
 C3DVectorWord() { }
 C3DVectorWord(const C3DVector& vect) { X=(short)vect.X; Y=(short)vect.Y; Z=(short)vect.Z; }
 bool operator==(const C3DVectorWord& rhs) const { return X==rhs.X && Y==rhs.Y && Z==rhs.Z; }
 C3DVector operator*(float scale) const { return C3DVector(X*scale,Y*scale,Z*scale); }
};
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
class C3DAnimationSequenceData::C3DVectorWord {
public:
 short X,Y,Z;
 C3DVector operator*(float scale) const { return C3DVector(X*scale,Y*scale,Z*scale); }
};
#endif
#ifdef FABLE_RETAIL_ANIMATION_KEYFRAME_TRACKS_PASS30
#include "engine/C3DKeyframe.h"
// Inline-budget: both bone accessors expand this complete sequence interpolation.
FABLE_FORCEINLINE void C3DAnimationSequenceData::InlineGetKeyframeInterpolate(C3DKeyframe& dest, unsigned long frame, float fraction) const
{
    unsigned long next_frame = (frame + 1 < FrameCount) ? frame + 1 : 0;
    float w1 = 1.0f - fraction;
    float w2 = fraction;
    if (RotationTrackMode & 2)
    {
        unsigned long index = frame;
        unsigned long next_index = next_frame;
        if (RotationTrackMode == 3)
        {
            index = PalettedRotationTrack[frame];
            next_index = PalettedRotationTrack[next_frame];
            if (index == next_index)
            {
                dest.Rotation = RotationTrack[index];
                goto rotation_done;
            }
        }
        dest.Rotation.SetSlerp(RotationTrack[index], RotationTrack[next_index], w2);
    }
    else if (RotationTrackMode == 1)
        dest.Rotation = RotationTrack[0];
    else
        dest.Rotation.SetIdentity();
rotation_done:
    if (PositionTrackMode & 2)
    {
        unsigned long index = frame;
        unsigned long next_index = next_frame;
        if (PositionTrackMode == 3)
        {
            index = PalettedPositionTrack[frame];
            next_index = PalettedPositionTrack[next_frame];
        }
        const C3DVectorWord& position = PositionTrack[index];
        const C3DVectorWord& next_position = PositionTrack[next_index];
        dest.SetPosition(position * (w1 * PositionFactor) + next_position * (w2 * PositionFactor));
    }
    else if (PositionTrackMode == 1)
        dest.SetPosition(PositionTrack[0] * PositionFactor);
    else
        dest.SetPosition(C3DVector(0.0f, 0.0f, 0.0f));
}
#endif
#endif
