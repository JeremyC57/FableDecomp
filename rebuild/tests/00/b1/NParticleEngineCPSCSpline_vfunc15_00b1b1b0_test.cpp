// BEGIN exact source snapshot SHA256 053bedae8bf9bcd63226535c2e9525d774e896a3509e8790091e7874f8be4522
// NParticleEngine::CPSCSpline::ReadBinary(CDataInputStream&), recovered PDB signature.
// Retail 00b1b1b0 reads SplineBounce, ScaleSplineSpeed, SplineAnimSpeed then base.
// PDB derived members +3d,+3e,+40 become retail +39,+3a,+3c after base shrinks4.
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
#pragma intrinsic(memcpy)
struct InputCalls:CDataInputStream {
 void ReadWithSrcChunkOverflow(unsigned char*,long); //00993ca0
 static __forceinline bool IsValidPosition(__int64 p){return p>=0&&p<=0x7fffffff;}
 __forceinline void Read(void* buffer,long size){
  __int64 next=(__int64)(StreamPos+size);
  if(!IsValidPosition(next))return;
  if(SourceChunkBytesRemaining>=size){
   memcpy(buffer,CurrentSourcePtr,size);
   CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;
   SourceChunkBytesRemaining-=size;
   StreamPos+=size;
  } else ReadWithSrcChunkOverflow((unsigned char*)buffer,size);
 }
};
static __forceinline void TransferBool(InputCalls& in,bool* out){unsigned char value;in.Read(&value,1);*out=value!=0;}
static __forceinline void TransferFloat(InputCalls& in,float* out){float value;in.Read(&value,4);*out=value;}
namespace NParticleEngine {
class CPSCSplineBase { public:unsigned char BaseData[0x34];void ReadBinary(InputCalls&);}; //00b22b40
class CPSCSpline:public CPSCSplineBase { public:
 float SplineCurrentPosition;bool ReverseSpline,SplineBounce,ScaleSplineSpeed;unsigned char Align;
 float SplineAnimSpeed;
 void ReadBinary(InputCalls&);
};
void CPSCSpline::ReadBinary(InputCalls& in){
 TransferBool(in,&SplineBounce);TransferBool(in,&ScaleSplineSpeed);TransferFloat(in,&SplineAnimSpeed);
 CPSCSplineBase::ReadBinary(in);
}
}

// END exact source snapshot
#include <stdio.h>
static int slow,basecalls;static unsigned char* fallback;
void InputCalls::ReadWithSrcChunkOverflow(unsigned char* p,long n){++slow;memcpy(p,fallback,n);fallback+=n;StreamPos+=n;}
void NParticleEngine::CPSCSplineBase::ReadBinary(InputCalls& in){++basecalls;if(in.StreamPos!=6)basecalls=99;}
int main(){unsigned char data[6]={0,7,0,0,0,0};float speed=2.5f;memcpy(data+2,&speed,4);
 InputCalls in;in.StreamPos=0;in.CurrentSourcePtr=data;in.SourceChunkBytesRemaining=6;
 NParticleEngine::CPSCSpline s;s.ReverseSpline=true;s.ReadBinary(in);
 if(s.SplineBounce||!s.ScaleSplineSpeed||s.SplineAnimSpeed!=2.5f||!s.ReverseSpline||basecalls!=1||slow||in.SourceChunkBytesRemaining)return 1;
 fallback=data;in.StreamPos=0;in.SourceChunkBytesRemaining=0;s.ReadBinary(in);
 if(slow!=3||basecalls!=2||s.SplineBounce||!s.ScaleSplineSpeed||s.SplineAnimSpeed!=2.5f)return 2;
 puts("00b1b1b0_TEST PASS");return 0;}