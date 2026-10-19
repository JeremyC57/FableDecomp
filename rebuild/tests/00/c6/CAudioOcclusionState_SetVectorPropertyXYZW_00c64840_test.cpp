#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// CAudioOcclusionState::SetOcclusion(parameters) -- validates and updates EAX-style occlusion parameters.
// Retail00c64840 length245; propagated CEntity/SetVectorProperty_XYZW label is wrong.
// Caller00c6859f validates16-byte payload. Adjacent00c647b0 accepts8-byte obstruction,
// 00c64940 accepts8-byte exclusion. Float pools122dedc=0,122ded8=1,1237f0c=10.
// Native fields dirty+4, owning audio context+14, occlusion record+30; context error+3d4.
// No applicable engine header for this third-party audio state is available.
struct CAudioPropertyContext {unsigned char UnrelatedAudioState[0x3d4];int Error;};
struct COcclusionParameters {int Level;float LowFrequencyRatio,RoomRatio,DirectRatio;};
struct CAudioOcclusionState {
 unsigned char UnrelatedPrefix[4];unsigned int DirtyFlags;unsigned char UnrelatedProperties[12];
 CAudioPropertyContext* Context;unsigned char EarlierProperties[0x18];COcclusionParameters Occlusion;
 int SetOcclusion(const COcclusionParameters& p);
};
int CAudioOcclusionState::SetOcclusion(const COcclusionParameters& p){
 if(p.Level < -10000 || p.Level > 0 || p.LowFrequencyRatio < 0.0f || p.LowFrequencyRatio > 1.0f ||
 p.RoomRatio < 0.0f || p.RoomRatio > 10.0f || p.DirectRatio < 0.0f || p.DirectRatio > 10.0f){
 Context->Error=-2;return -3;
 }
 if(p.Level!=Occlusion.Level || p.LowFrequencyRatio!=Occlusion.LowFrequencyRatio || p.RoomRatio!=Occlusion.RoomRatio || p.DirectRatio!=Occlusion.DirectRatio){
 Occlusion.Level=p.Level;Occlusion.LowFrequencyRatio=p.LowFrequencyRatio;Occlusion.RoomRatio=p.RoomRatio;Occlusion.DirectRatio=p.DirectRatio;DirtyFlags|=0x400000;
 }
 return 0;
}



#include <stdio.h>
#include <stddef.h>
#include <string.h>
#ifdef _MSC_VER
typedef char ContextOffset[(offsetof(CAudioOcclusionState,Context)==0x14)?1:-1];
typedef char OcclusionOffset[(offsetof(CAudioOcclusionState,Occlusion)==0x30)?1:-1];
typedef char ErrorOffset[(offsetof(CAudioPropertyContext,Error)==0x3d4)?1:-1];
#endif
static int bad;
static void Check(bool b){if(!b)bad=1;}
static void Run(COcclusionParameters p,bool valid){CAudioPropertyContext c;CAudioOcclusionState x;x.Context=&c;c.Error=77;x.DirtyFlags=0x81;
 COcclusionParameters old={-100,0.5f,2.0f,3.0f};x.Occlusion=old;int r=x.SetOcclusion(p);
 if(valid){Check(r==0&&c.Error==77&&x.DirtyFlags==0x400081);Check(memcmp(&x.Occlusion,&p,sizeof(p))==0);}
 else {Check(r==-3&&c.Error==-2&&x.DirtyFlags==0x81);Check(memcmp(&x.Occlusion,&old,sizeof(old))==0);}}
int main(){COcclusionParameters p={-10000,0,0,0};Run(p,true);p.Level=0;p.LowFrequencyRatio=1;p.RoomRatio=10;p.DirectRatio=10;Run(p,true);
 p.Level=-10001;Run(p,false);p.Level=1;Run(p,false);p.Level=-1;
 p.LowFrequencyRatio=-0.01f;Run(p,false);p.LowFrequencyRatio=1.01f;Run(p,false);p.LowFrequencyRatio=0.5f;
 p.RoomRatio=-0.01f;Run(p,false);p.RoomRatio=10.01f;Run(p,false);p.RoomRatio=1;
 p.DirectRatio=-0.01f;Run(p,false);p.DirectRatio=10.01f;Run(p,false);p.DirectRatio=1;
 unsigned int nanBits=0x7fc00000;float nanValue;memcpy(&nanValue,&nanBits,4);
 p.LowFrequencyRatio=nanValue;Run(p,true);p.RoomRatio=nanValue;Run(p,true);p.DirectRatio=nanValue;Run(p,true);
 CAudioPropertyContext c;CAudioOcclusionState x;x.Context=&c;c.Error=91;x.DirtyFlags=8;p.Level=-20;p.LowFrequencyRatio=.25f;p.RoomRatio=1;p.DirectRatio=2;x.Occlusion=p;
 Check(x.SetOcclusion(p)==0&&x.DirtyFlags==8&&c.Error==91);Check(x.SetOcclusion(x.Occlusion)==0&&x.DirtyFlags==8);
 printf("00c64840_TEST %s\n",bad?"FAIL":"PASS");return bad;}