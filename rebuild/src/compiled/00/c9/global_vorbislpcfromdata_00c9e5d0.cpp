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