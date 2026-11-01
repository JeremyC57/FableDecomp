// libvorbis::Laguerre_With_Deflation(float* a, int ord, float* r) -- libvorbis lsp.c: find the ord real roots of the polynomial a by Laguerre iteration with forward deflation, in double precision; -1 when the discriminant goes negative
// Leaf: _Laguerre_With_Deflation@12
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3B80, 718 bytes. libvorbis 1.0.1 lib/lsp.c static helper of vorbis_lpc_to_lsp inside
//   libpk's statically linked Xiph encoder ("Xiph.Org libVorbis I 20030909" @ 0x012BF240).
// Identity (proven): the only callers are 0x00CA449E and 0x00CA44B5 in vorbis_lpc_to_lsp 0x00CA4160 (the two
//   `if(Laguerre_With_Deflation(g1,g1_order,g1r) || Laguerre_With_Deflation(g2,g2_order,g2r)) return(-1);` calls before the
//   two Newton_Raphson 0x00CA3F60 calls, landed with this same recipe). The body is the lsp.c text verbatim:
//   alloca((ord+1)*sizeof(double)) through __alloca_probe 0x00BFEA30, defl[i]=a[i], the m = ord..1 loop with the Laguerre
//   step (EPSILON 1e-6 @ 0x012BF370 / -1e-6 @ 0x012BF368 clamps of the denominator, 10e-12 @ 0x012BF360 convergence test,
//   -1.0 @ 0x01236700 / 0.0 @ 0x0122ED70), r[m-1]=new, forward deflation `defl[i-1]+=new*defl[i]`, then `defl++`.
// Convention: VC7.1's private convention for a TU-local static with a same-TU caller (ord in eax; retail pops its two stack
//   arguments `ret 8`, so the static is `__stdcall`: the landed Newton_Raphson 0x00CA3F60 / cheby 0x00CA3A40 precedent).
// Label: the catalog's "_global::Environment_SolveLinearSystemGaussian"-style stand-in is a placeholder; the real name is the
//   lsp.c static `Laguerre_With_Deflation`.
#include <malloc.h>
#include <math.h>
#define alloca _alloca
#define EPSILON 10e-7
extern "C" {
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
}