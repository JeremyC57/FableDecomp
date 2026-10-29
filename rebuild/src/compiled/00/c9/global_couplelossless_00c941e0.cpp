// 00c941e0 couple_lossless
// Leaf: @couple_lossless@16
// Flags: /Gr
// Original libvorbis psy.c; BSD-style terms in reference COPYING.
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" double __cdecl fabs(double);
#ifdef _MSC_VER
#pragma intrinsic(fabs)
#else
#define __fastcall
#endif
// Fidelity: Pointer parameters follow the native ECX=qB, EDX=qA contract.
extern "C" void __fastcall couple_lossless(float A, float B, 
			    float *qB, float *qA){
  int test1=fabs(*qA)>fabs(*qB);
  test1-= fabs(*qA)<fabs(*qB);
  
  if(!test1)test1=((fabs(A)>fabs(B))<<1)-1;
  if(test1==1){
    *qB=(*qA>0.f?*qA-*qB:*qB-*qA);
  }else{
    float temp=*qB;  
    *qB=(*qB>0.f?*qA-*qB:*qB-*qA);
    *qA=temp;
  }

  if(*qB>fabs(*qA)*1.9999f){
    *qB= -fabs(*qA)*2.f;
    *qA= -*qA;
  }
}