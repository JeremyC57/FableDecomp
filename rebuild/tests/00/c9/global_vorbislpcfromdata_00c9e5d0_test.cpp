#include <stdio.h>
// 00c9e5d0 vorbis_lpc_from_data
// Leaf: @vorbis_lpc_from_data@16
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
extern "C" float __fastcall vorbis_lpc_from_data(float *data,float *lpci,int n,int m){
  double *aut=(double*)alloca(sizeof(*aut)*(m+1));
  double *lpc=(double*)alloca(sizeof(*lpc)*(m));
  double error;
  int i,j;

  /* autocorrelation, p+1 lag coefficients */
  j=m+1;
  while(j--){
    double d=0; /* double needed for accumulator depth */
    for(i=j;i<n;i++)d+=(double)data[i]*data[i-j];
    aut[j]=d;
  }
  
  /* Generate lpc coefficients from autocorr values */

  error=aut[0];
  
  for(i=0;i<m;i++){
    double r= -aut[i+1];

    if(error==0){
      memset(lpci,0,m*sizeof(*lpci));
      return 0;
    }

    /* Sum up this iteration's reflection coefficient; note that in
       Vorbis we don't save it.  If anyone wants to recycle this code
       and needs reflection coefficients, save the results of 'r' from
       each iteration. */

    for(j=0;j<i;j++)r-=lpc[j]*aut[i-j];
    r/=error; 

    /* Update LPC coefficients and total error */
    
    lpc[i]=r;
    for(j=0;j<i/2;j++){
      double tmp=lpc[j];

      lpc[j]+=r*lpc[i-1-j];
      lpc[i-1-j]+=r*tmp;
    }
    if(i%2)lpc[j]+=lpc[j]*r;

    error*=1.f-r*r;
  }

  for(j=0;j<m;j++)lpci[j]=(float)lpc[j];

  /* we need the error value to know how big an impulse to hit the
     filter with later */
  
  return error;
}

extern "C" void abort();
static unsigned cases;static void check(bool p){if(!p){printf("LPC case %u failed\n",cases);abort();}}
static double absolute(double x){return x<0?-x:x;}
static void check_close(double actual,double expected){check(actual==actual);double error=absolute(actual-expected);double tolerance=(1.0+absolute(expected))/32768.0;check(error<=tolerance);}
// Independent oracle: solve the Yule-Walker Toeplitz system by pivoted Gaussian elimination.
static double reference(const float*input,int n,int order,double*coeff){double autocorrelation[9];for(int lag=0;lag<=order;++lag){autocorrelation[lag]=0;for(int k=lag;k<n;++k)autocorrelation[lag]+=(double)input[k]*input[k-lag];}if(autocorrelation[0]==0){for(int k=0;k<order;++k)coeff[k]=0;return 0;}double matrix[8][9];for(int row=0;row<order;++row){for(int col=0;col<order;++col)matrix[row][col]=autocorrelation[row>col?row-col:col-row];matrix[row][order]=-autocorrelation[row+1];}for(int col=0;col<order;++col){int pivot=col;for(int row=col+1;row<order;++row)if(absolute(matrix[row][col])>absolute(matrix[pivot][col]))pivot=row;check(absolute(matrix[pivot][col])>1.0/1099511627776.0);for(int k=col;k<=order;++k){double tmp=matrix[col][k];matrix[col][k]=matrix[pivot][k];matrix[pivot][k]=tmp;}double scale=matrix[col][col];for(int k=col;k<=order;++k)matrix[col][k]/=scale;for(int row=0;row<order;++row)if(row!=col){double factor=matrix[row][col];for(int k=col;k<=order;++k)matrix[row][k]-=factor*matrix[col][k];}}double energy=autocorrelation[0];for(int k=0;k<order;++k){coeff[k]=matrix[k][order];energy+=coeff[k]*autocorrelation[k+1];}return energy;}
int main(){const int sizes[]={1,2,4,8,16,32};for(int t=0;t<6;++t){int n=sizes[t];int limit=n-1<8?n-1:8;for(int order=0;order<=limit;++order)for(int shape=0;shape<8;++shape)for(int seed=1;seed<=7;++seed){float input[64],original[64],output[12];for(int i=0;i<64;++i){int v=shape==0?0:(shape==1?(i==seed%n?8:0):(shape==2?seed:(shape==3?(i%2?-4:4):(shape==4?(i%8-4):(shape==5?((i*37+seed*11)%17-8):(shape==6?((i==0||i==n-1)?4:0):((i*i+seed*7+i*seed)%23-11)))))));input[i]=v*0.125f;original[i]=input[i];}for(int i=0;i<12;++i)output[i]=128.0f;double expected[8];double energy=reference(input,n,order,expected);float actual=vorbis_lpc_from_data(input,output+1,n,order);check_close(actual,energy);for(int i=0;i<order;++i)check_close(output[i+1],expected[i]);check(output[0]==128.0f);for(int i=order+1;i<12;++i)check(output[i]==128.0f);for(int i=0;i<64;++i)check(input[i]==original[i]);++cases;}}check(cases==1848);puts("00c9e5d0_TEST PASS");return 0;}