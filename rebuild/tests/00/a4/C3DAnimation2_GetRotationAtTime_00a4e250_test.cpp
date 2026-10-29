#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST
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

CBaseClass::~CBaseClass(){}CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}C3DAnimationStats::~C3DAnimationStats(){}CMovableResource::~CMovableResource(){}
static const C3DAnimationSequenceData*expectedThis;static unsigned long expectedTime,rotationBits[4];static unsigned calls,errors;
static unsigned long bits(float x){unsigned long n;memcpy(&n,&x,4);return n;}static float value(unsigned long n){float x;memcpy(&x,&n,4);return x;}
void C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&key,float time)const{++calls;if(this!=expectedThis||bits(time)!=expectedTime)++errors;memset(&key,0x5a,sizeof(key));memcpy(&key.Rotation,rotationBits,16);}
int main(){unsigned long durations[]={0,0x80000000UL,0x3f800000UL,0x41200000UL,0xbf800000UL,0x7f800000UL,0x7fc12345UL};unsigned long times[]={0,0x80000000UL,0xbf800000UL,0xc1200000UL,0x3f000000UL,0x3f800000UL,0x41200000UL,0x7f800000UL,0xff800000UL,0x7fc54321UL,1};unsigned long patterns[][4]={{0,0,0,0x3f800000UL},{1,2,3,4},{0x80000000UL,0xff800000UL,0x7f800000UL,0x7fc12345UL},{0x3f800000UL,0x40000000UL,0x40400000UL,0x40800000UL},{0xffffffffUL,0xdeadbeefUL,0x80000001UL,0x7f7fffffUL},{0xbf800000UL,0xc0000000UL,0xc0400000UL,0xc0800000UL},{0x12345678UL,0x23456789UL,0x3456789aUL,0x456789abUL},{0x7fc00001UL,0x7fc00002UL,0x7fc00003UL,0x7fc00004UL}};unsigned total=0;for(unsigned d=0;d<7;++d)for(unsigned tm=0;tm<11;++tm)for(unsigned present=0;present<2;++present)for(unsigned pat=0;pat<8;++pat)for(unsigned rep=0;rep<3;++rep){C3DAnimation2 animation;C3DAnimationSequenceData data;C3DAnimationSequence sequence;sequence.Data=&data;sequence.ParentIndex=-7;sequence.Shared=true;animation.Duration=value(durations[d]);animation.MovementTrack.Data=present?&sequence:0;animation.MovementTrack.Info=0;if((char*)&animation.MovementTrack-(char*)&animation!=64||(char*)(CMovableResource*)&animation-(char*)&animation!=44)return 1;unsigned char before[sizeof(animation)],seqBefore[sizeof(sequence)],dataBefore[sizeof(data)];memcpy(before,&animation,sizeof(animation));memcpy(seqBefore,&sequence,sizeof(sequence));memcpy(dataBefore,&data,sizeof(data));float time=value(times[tm]);float clamped=time;if(time<0.0f)clamped=0.0f;else if(time>animation.Duration)clamped=animation.Duration;expectedTime=bits(clamped);expectedThis=&data;memcpy(rotationBits,patterns[pat],16);calls=errors=0;CQuaternion result=animation.C3DAnimation2::GetRotationAtTime(time);unsigned long expected[4]={0,0,0,0x3f800000UL};if(present)memcpy(expected,rotationBits,16);if(memcmp(&result,expected,16)||errors||calls!=present)return 2;if(memcmp(before,&animation,sizeof(animation))||memcmp(seqBefore,&sequence,sizeof(sequence))||memcmp(dataBefore,&data,sizeof(data)))return 3;++total;}if(total!=3696)return 4;puts("00a4e250_TEST PASS");return 0;}