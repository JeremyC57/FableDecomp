// NParticleEngine::CPSCSpline::ReadBinary(CDataInputStream&), recovered PDB signature.
// Retail 00b1b1b0 reads SplineBounce, ScaleSplineSpeed, SplineAnimSpeed then base.
// PDB derived members +3d,+3e,+40 become retail +39,+3a,+3c after base shrinks4.
#define FABLE_CDATAINPUTSTREAM_FRIEND struct InputCalls   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
#pragma intrinsic(memcpy)
struct InputCalls:CDataInputStream {
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