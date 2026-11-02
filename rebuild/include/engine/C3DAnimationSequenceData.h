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
#endif
