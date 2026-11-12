#ifndef FABLE_ENGINE_C3DANIMATIONSEQUENCEDATA_H
#define FABLE_ENGINE_C3DANIMATIONSEQUENCEDATA_H
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
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCE_GETKEYFRAME
 // Retail opt-in (chunk 2014, row 0x00A4E300): Ego_r.pdb / FableWin.pdb ?GetKeyframeInterpolate@C3DAnimationSequenceData@@QBEXAAVC3DKeyframe@@M@Z
 // (retail 0x00A4DEA0, out of line, `ret 8`; landed row C3DAnimationSequenceData_GetKeyframeInterpolate_00a4dea0, bound spelling of 0x00A4E250).
 void GetKeyframeInterpolate(C3DKeyframe& keyframe, float time) const;
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCE_INITIALISE
 // Retail opt-in (chunk 2014): Ego_r.pdb ?InitialiseAsConstant@C3DAnimationSequenceData@@QAEXABVCMatrix3x4@@KMAAVCSimpleMemoryPool@@@Z
 // (retail 0x00A4FD70, out of line, `ret 10h`).
 void InitialiseAsConstant(const CMatrix3x4& matrix, unsigned long frame_count, float samples_per_second, CSimpleMemoryPool& memory_pool);
#endif
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
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
class C3DAnimationSequenceData::C3DVectorWord {
public:
 short X,Y,Z;
 C3DVector operator*(float scale) const { return C3DVector(X*scale,Y*scale,Z*scale); }
};
#endif
#endif
