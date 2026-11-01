// Test for 0x00CA3B80 Laguerre_With_Deflation (libvorbis 1.0.1 lsp.c): roots of (x-1)(x-2)(x-3) and a defective input.
#include <stdio.h>
#ifndef _MSC_VER
#define __declspec(x)
#define __stdcall
#define __cdecl
#define _alloca __builtin_alloca
#endif
#include <malloc.h>
#include <math.h>
#define alloca _alloca
#define EPSILON 10e-7
static __declspec(noinline) int __stdcall Laguerre_With_Deflation(float *a,int ord,float *r){
  int i,m;
  double lastdelta=0.f;
  double *defl=(double*)alloca(sizeof(*defl)*(ord+1));
  for(i=0;i<=ord;i++)defl[i]=a[i];

  for(m=ord;m>0;m--){
    double new_=0.f,delta;

    /* iterate a root */
    while(1){
      double p=defl[m],pp=0.f,ppp=0.f,denom;
      
      /* eval the polynomial and its first two derivatives */
      for(i=m;i>0;i--){
	ppp = new_*ppp + pp;
	pp  = new_*pp  + p;
	p   = new_*p   + defl[i-1];
      }
      
      /* Laguerre's method */
      denom=(m-1) * ((m-1)*pp*pp - m*p*ppp);
      if(denom<0)
	return(-1);  /* complex root!  The LPC generator handed us a bad filter */

      if(pp>0){
	denom = pp + sqrt(denom);
	if(denom<EPSILON)denom=EPSILON;
      }else{
	denom = pp - sqrt(denom);
	if(denom>-(EPSILON))denom=-(EPSILON);
      }

      delta  = m*p/denom;
      new_   -= delta;

      if(delta<0.f)delta*=-1;

      if(fabs(delta/new_)<10e-12)break; 
      lastdelta=delta;
    }

    r[m-1]=new_;

    /* forward deflation */
    
    for(i=m;i>0;i--)
      defl[i-1]+=new_*defl[i];
    defl++;

  }
  return(0);
}
// Same-TU caller (the two Laguerre_With_Deflation calls of lsp.c vorbis_lpc_to_lsp, 0x00CA449E / 0x00CA44B5), present so
// the static gets retail's private convention. Not graded.
int __cdecl Laguerre_With_Deflation_caller(float* a, int ord, float* r) { return Laguerre_With_Deflation(a, ord, r); }
int main(void)
{
    int ok = 1;
    /* (x-1)(x-2)(x-3) = x^3 - 6x^2 + 11x - 6, coefficients low to high */
    float a[4]; a[0] = -6.f; a[1] = 11.f; a[2] = -6.f; a[3] = 1.f;
    float r[3]; r[0] = r[1] = r[2] = 0.f;
    int ret = Laguerre_With_Deflation_caller(a, 3, r);
    if (ret != 0) ok = 0;
    int seen[4] = {0, 0, 0, 0};
    for (int i = 0; i < 3; i++) {
        int k = (int)(r[i] + 0.5f);
        if (k < 1 || k > 3 || r[i] - (float)k > 1e-3f || (float)k - r[i] > 1e-3f) ok = 0; else seen[k]++;
    }
    if (seen[1] != 1 || seen[2] != 1 || seen[3] != 1) ok = 0;
    /* x^2 + 1: complex roots, the deflation never converges to a real root */
    float b[3]; b[0] = 1.f; b[1] = 0.f; b[2] = 1.f;
    float rb[2]; rb[0] = rb[1] = 0.f;
    if (Laguerre_With_Deflation_caller(b, 2, rb) != -1) ok = 0;
    printf(ok ? "00ca3b80_TEST PASS\n" : "00ca3b80_TEST FAIL\n");
    return 0;
}