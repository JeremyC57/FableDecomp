// 00a4e250 C3DAnimation2::GetRotationAtTime
// Leaf: ?GetRotationAtTime@C3DAnimation2@@UBE?AVCQuaternion@@M@Z
// Flags:
#include "rebuild_abi.h"
#define C3DAnimation2 C3DAnimation2Fields
#include "engine/C3DAnimation2.h"
#undef C3DAnimation2
#define CMovableResource CMovableResourceFields
#include "engine/CMovableResource.h"
#undef CMovableResource
class CQuaternion{public:float X,Y,Z,W;CQuaternion(){}};class C4DVector{public:float X,Y,Z,W;};class C3DKeyframe{public:CQuaternion Rotation;C4DVector Position,Scaling;C3DKeyframe(){}};

class C3DAnimationSequenceData{public:class C3DVectorWord;char*BoneName;float SamplesPerSecond;unsigned short FrameCount;unsigned char BoneType:5,EnabledFlag:1,AnimatedFlag:1,IdentityFlag:1;unsigned char RotationTrackMode:2,PositionTrackMode:2,ScalingTrackMode:2;float PositionFactor,ScalingFactor;CQuaternion*RotationTrack;unsigned short RotationTrackSize,PositionTrackSize,PalettedRotationTrackSize,PalettedPositionTrackSize;C3DVectorWord*PositionTrack;unsigned char*PalettedRotationTrack,*PalettedPositionTrack;
 void GetKeyframeInterpolate(class C3DKeyframe&,float)const;
 C3DAnimationSequenceData(){BoneName=0;RotationTrack=0;RotationTrackSize=0;PositionTrackSize=0;PalettedRotationTrackSize=0;PalettedPositionTrackSize=0;PositionTrack=0;PalettedRotationTrack=0;PalettedPositionTrack=0;FrameCount=0;IdentityFlag=0;}
};
class C3DAnimationSequence{public:C3DAnimationSequenceData*Data;short ParentIndex;bool Shared;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;T*Get()const{return Data;}T*operator->()const{return Data;}};
class CBaseClass{public:virtual~CBaseClass();};class CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer*IntelligentPointer;protected:CBaseObject();virtual~CBaseObject();};
// Layout: original statistics base and duration are restored; unread statistics and derived receiver state are opaque. Retail secondary-base displacement is44, unlike donor sizeof48.
class C3DAnimationStats:public CBaseObject{unsigned char unreadType[4];public:float Duration;private:unsigned char unreadState[28];public:virtual~C3DAnimationStats();};class CMovableResource{public:virtual~CMovableResource();};
class C3DAnimation2:public C3DAnimationStats,public CMovableResource{unsigned char unreadState[16];public:CCountedPointer<C3DAnimationSequence>MovementTrack;virtual CQuaternion GetRotationAtTime(float)const;};
CQuaternion C3DAnimation2::GetRotationAtTime(float time)const{float duration=Duration;if(time<0.0f)time=0.0f;else if(time>duration)time=duration;CQuaternion rotation;if(MovementTrack.Get()){C3DKeyframe keyframe;MovementTrack->Data->GetKeyframeInterpolate(keyframe,time);rotation=keyframe.Rotation;}else{rotation.X=0.0f;rotation.Y=0.0f;rotation.Z=0.0f;rotation.W=1.0f;}return rotation;}
FABLE_STATIC_ASSERT(sizeof(CQuaternion)==16);FABLE_STATIC_ASSERT(sizeof(C3DKeyframe)==48);FABLE_STATIC_ASSERT(sizeof(C3DAnimationStats)==44);FABLE_STATIC_ASSERT(sizeof(CMovableResource)==sizeof(CMovableResourceFields));