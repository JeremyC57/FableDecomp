// 00a53440 CSimpleMemoryPool::New<C3DAnimationSequenceData>
// Leaf: ??$New@VC3DAnimationSequenceData@@@CSimpleMemoryPool@@QAEPAVC3DAnimationSequenceData@@K@Z
// Flags:
#include <new>
#include "rebuild_abi.h"
#define CSimpleMemoryPool CSimpleMemoryPoolFields
#include "engine/CSimpleMemoryPool.h"
#undef CSimpleMemoryPool
class CQuaternion;
class C3DAnimationSequenceData{private:class C3DVectorWord;char*BoneName;float SamplesPerSecond;unsigned short FrameCount;unsigned char BoneType:5,EnabledFlag:1,AnimatedFlag:1,IdentityFlag:1;unsigned char RotationTrackMode:2,PositionTrackMode:2,ScalingTrackMode:2;float PositionFactor,ScalingFactor;CQuaternion*RotationTrack;unsigned short RotationTrackSize,PositionTrackSize,PalettedRotationTrackSize,PalettedPositionTrackSize;C3DVectorWord*PositionTrack;unsigned char*PalettedRotationTrack,*PalettedPositionTrack;
 public:C3DAnimationSequenceData(){BoneName=0;RotationTrack=0;RotationTrackSize=0;PositionTrackSize=0;PalettedRotationTrackSize=0;PalettedPositionTrackSize=0;PositionTrack=0;PalettedRotationTrack=0;PalettedPositionTrack=0;FrameCount=0;IdentityFlag=0;}
};
// Layout: only the original pool receiver and allocation API are used; unread state after its true noncopyable base is retained as opaque storage.
class CSimpleMemoryPool:public CSimpleMemoryPoolFields{public:void*Allocate(unsigned long);
 template<class T>T*New(unsigned long count){T*data;if(count){unsigned __int64 bytes=(unsigned __int64)count*sizeof(T);if((unsigned long)(bytes>>32)<=0UL&&(unsigned long)bytes<=0xffffffffUL)data=static_cast<T*>(Allocate(count*sizeof(T)));else data=0;}else data=0;T*cursor=data;for(unsigned long remaining=count;remaining>0;--remaining,++cursor)if(cursor)new(cursor)T;return data;}
};
template C3DAnimationSequenceData*CSimpleMemoryPool::New<C3DAnimationSequenceData>(unsigned long);
FABLE_STATIC_ASSERT(sizeof(CSimpleMemoryPool)==sizeof(CSimpleMemoryPoolFields));
FABLE_STATIC_ASSERT(sizeof(C3DAnimationSequenceData)==44);