#include <stdio.h>
// 00c9e8f0 vorbis_lpc_predict
// Leaf: @vorbis_lpc_predict@20
// Flags: /Gr
/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2002             *
 * by the XIPHOPHORUS Company http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

  function: LPC low level routines
  last mod: $Id: lpc.c,v 1.37 2003/03/08 07:15:32 xiphmont Exp $

 ********************************************************************/

/* Some of these routines (autocorrelator, LPC coefficient estimator)
   are derived from code written by Jutta Degener and Carsten Bormann;
   thus we include their copyright below.  The entirety of this file
   is freely redistributable on the condition that both of these
   copyright notices are preserved without modification.  */

/* Preserved Copyright: *********************************************/

/* Copyright 1992, 1993, 1994 by Jutta Degener and Carsten Bormann,
Technische Universita"t Berlin

Any use of this software is permitted provided that this notice is not
removed and that neither the authors nor the Technische Universita"t
Berlin are deemed to have made any representations as to the
suitability of this software for any purpose nor are held responsible
for any defects of this software. THERE IS ABSOLUTELY NO WARRANTY FOR
THIS SOFTWARE.

As a matter of courtesy, the authors request to be informed about uses
this software has found, about bugs in this software, and about any
improvements that may be of general interest.

Berlin, 28.11.1994
Jutta Degener
Carsten Bormann

*********************************************************************/

#include <string.h>
#ifdef _MSC_VER
#include <malloc.h>
#pragma intrinsic(memset)
#else
#include <alloca.h>
#define __fastcall
#endif
extern "C" void __fastcall vorbis_lpc_predict(float *coeff,float *prime,int m,
                     float *data,long n){

  /* in: coeff[0...m-1] LPC coefficients 
         prime[0...m-1] initial values (allocated size of n+m-1)
    out: data[0...n-1] data samples */

  long i,j,o,p;
  float y;
  float *work=(float*)alloca(sizeof(*work)*(m+n));

  if(!prime)
    for(i=0;i<m;i++)
      work[i]=0.f;
  else
    for(i=0;i<m;i++)
      work[i]=prime[i];

  for(i=0;i<n;i++){
    y=0;
    o=i;
    p=m;
    for(j=0;j<m;j++)
      y-=work[o++]*coeff[--p];
    
    data[i]=work[o]=y;
  }
}

extern "C" void abort();
static unsigned cases;
static void check(bool p){if(!p){fprintf(stderr,"prediction case %u failed\n",cases);abort();}}
static double magnitude(double x){return x<0?-x:x;}
// Independent oracle shifts a fixed-size history and applies lag coefficients.
static void reference(const float*coeff,const float*prime,int m,float*out,int n){
 float history[8];for(int k=0;k<m;++k)history[k]=prime?prime[k]:0.0f;
 for(int i=0;i<n;++i){double sum=0;for(int k=0;k<m;++k)sum-=(double)history[k]*coeff[m-k-1];out[i]=(float)sum;for(int k=1;k<m;++k)history[k-1]=history[k];if(m)history[m-1]=out[i];}
}
int main(){
 const int lengths[]={0,1,2,3,8,17,32};
 for(int m=0;m<=8;++m)for(int shape=0;shape<5;++shape)for(int seed=0;seed<7;++seed)for(int nullprime=0;nullprime<2;++nullprime)for(int t=0;t<7;++t){
 int n=lengths[t];float coeff[10],savedCoeff[10],prime[10],savedPrime[10],out[35],expected[32];
 for(int k=0;k<10;++k){coeff[k]=shape==0?0.0f:(shape==1?(k==0?0.5f:0.0f):(shape==2?(k==0?-0.5f:0.0f):(shape==3?((k+seed)%3-1)*0.0625f:((k*3+seed)%5-2)*0.03125f)));prime[k]=((k*7+seed*3)%17-8)*0.125f;savedCoeff[k]=coeff[k];savedPrime[k]=prime[k];}
 for(int k=0;k<35;++k)out[k]=64.0f;
 reference(coeff,nullprime?0:prime,m,expected,n);vorbis_lpc_predict(coeff,nullprime?0:prime,m,out+1,n);
 for(int k=0;k<n;++k)check(out[k+1]==out[k+1]&&magnitude((double)out[k+1]-expected[k])<=(1.0+magnitude(expected[k]))/1048576.0);
 check(out[0]==64.0f);for(int k=n+1;k<35;++k)check(out[k]==64.0f);for(int k=0;k<10;++k){check(coeff[k]==savedCoeff[k]);check(prime[k]==savedPrime[k]);}++cases;
 }
 check(cases==4410);puts("00c9e8f0_TEST PASS");return 0;
}