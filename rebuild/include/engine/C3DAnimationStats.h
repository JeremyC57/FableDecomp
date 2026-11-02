#ifndef FABLE_ENGINE_C3DANIMATIONSTATS_H
#define FABLE_ENGINE_C3DANIMATIONSTATS_H
#include "rebuild_abi.h"
#include "engine/CBaseObject.h"
#include "engine/EAnimationType.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationStats:public CBaseObject{public:
C3DAnimationStats(const C3DAnimationStats&);
C3DAnimationStats();
virtual ~C3DAnimationStats();
virtual C3DVector GetMovementOffsetAtTime(float) const=0;
virtual C3DVector GetMovementAtTime(float) const=0;
virtual bool HasRotation() const=0;
virtual CQuaternion GetRotationAtTime(float) const=0;
virtual void GetDeltaMovement(CMatrix3x4&,float,float) const=0;
void InitialiseStats(const C3DAnimFile&,EAnimationType);
private:EAnimationType AnimType;float Duration;bool LoopFlag,MovementFlag;unsigned char MovementVector[12];unsigned char _pad_1e[2];unsigned char TimingEvents[12];};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationStats)==0x2c);
#endif
