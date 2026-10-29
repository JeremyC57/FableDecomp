#include <stdio.h>
#include <stdlib.h>
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

static unsigned cases;
static void check(bool p){if(!p){fprintf(stderr,"coupling case %u failed\n",cases);abort();}}
static double magnitude(double x){return x<0?-x:x;}
int main(){
 for(int ai=-2;ai<=2;++ai)for(int bi=-2;bi<=2;++bi)
 for(int first=-32;first<=32;++first)for(int second=-32;second<=32;++second){
  float a=first*0.25f,b=second*0.25f;
  double aa=magnitude(a),bb=magnitude(b);
  bool firstWins=aa>bb||(aa==bb&&magnitude(ai)>magnitude(bi));
  float expectedM=firstWins?a:b;
  float expectedA=firstWins?(expectedM>0?a-b:b-a):(expectedM>0?a-b:b-a);
  if((double)expectedA>magnitude(expectedM)*(double)1.9999f){expectedA=(float)(-2*magnitude(expectedM));expectedM=-expectedM;}
  float firstBox[3]={4096.f,a,-4096.f},secondBox[3]={2048.f,b,-2048.f};
  couple_lossless((float)ai,(float)bi,secondBox+1,firstBox+1);
  float m=firstBox[1],ang=secondBox[1];
  check(m==expectedM&&ang==expectedA);
  float restoredA,restoredB;
  if(m>0){if(ang>0){restoredA=m;restoredB=m-ang;}else{restoredA=m+ang;restoredB=m;}}
  else{if(ang>0){restoredA=m;restoredB=m+ang;}else{restoredA=m-ang;restoredB=m;}}
  check(restoredA==a&&restoredB==b);
  check(firstBox[0]==4096.f&&firstBox[2]==-4096.f&&secondBox[0]==2048.f&&secondBox[2]==-2048.f);
  ++cases;
 }
 check(cases==105625);puts("00c941e0_TEST PASS");return 0;
}