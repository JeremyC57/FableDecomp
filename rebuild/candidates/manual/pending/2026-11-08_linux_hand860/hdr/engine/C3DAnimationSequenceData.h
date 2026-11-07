#ifndef FABLE_ENGINE_C3DANIMATIONSEQUENCEDATA_H
#define FABLE_ENGINE_C3DANIMATIONSEQUENCEDATA_H
#include "rebuild_abi.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationSequenceData{
public:
 class C3DVectorWord;
 C3DAnimationSequenceData():BoneName(0),RotationTrack(0),RotationTrackSize(0),PositionTrackSize(0),PalettedRotationTrackSize(0),PalettedPositionTrackSize(0),PositionTrack(0),PalettedRotationTrack(0),PalettedPositionTrack(0){FrameCount=0;IdentityFlag=false;}
 unsigned long GetFrameCount()const{return FrameCount;}float GetSamplesPerSecond()const{return SamplesPerSecond;}
 bool InitialiseFromFileData(const C3DAnimFileSequenceChunk&,CSimpleMemoryPool&);
private:
char* BoneName;float SamplesPerSecond;unsigned short FrameCount;
 unsigned char BoneType:5;unsigned char EnabledFlag:1;unsigned char AnimatedFlag:1;unsigned char IdentityFlag:1;
 unsigned char RotationTrackMode:2;unsigned char PositionTrackMode:2;unsigned char ScalingTrackMode:2;
 float PositionFactor,ScalingFactor;CQuaternion* RotationTrack;
 unsigned short RotationTrackSize,PositionTrackSize,PalettedRotationTrackSize,PalettedPositionTrackSize;
 C3DVectorWord* PositionTrack;unsigned char* PalettedRotationTrack;unsigned char* PalettedPositionTrack;
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationSequenceData)==0x2c);
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_VECTORWORD
// Retail opt-in (chunk 860, row 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData): the nested C3DVectorWord (Ego_r.pdb
// ??0C3DVectorWord@C3DAnimationSequenceData@@QAE@XZ, ??0C3DVectorWord@C3DAnimationSequenceData@@QAE@ABVC3DVector@@@Z,
// ??8C3DVectorWord@C3DAnimationSequenceData@@QBE_NABV01@@Z; no out-of-line copy in retail): three shorts (6 bytes, the
// 0x2AAAAAAB / 6 size divisions at 0x00A4F68D), built from a C3DVector by three __ftol2 truncations (0x00A4F654..0x00A4F66C)
// and compared X, Y, Z in turn (0x00A4F6B0..0x00A4F6CA).
#include "engine/C3DVector.h"
class C3DAnimationSequenceData::C3DVectorWord {
public:
    short X, Y, Z;
    C3DVectorWord() { }
    C3DVectorWord(const C3DVector& vector) : X((short)vector.X), Y((short)vector.Y), Z((short)vector.Z) { }
    bool operator==(const C3DVectorWord& other) const { return X == other.X && Y == other.Y && Z == other.Z; }
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationSequenceData::C3DVectorWord) == 6);
#endif
#endif
