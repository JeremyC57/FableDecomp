// libvorbis::Newton_Raphson(float* a, int ord, float* r) -- libvorbis lsp.c: polish the ord real roots r of the polynomial a by simultaneous Newton steps in double precision (Horner value and derivative), until the summed squared step is <= 1e-20; -1 after 41 passes, leaving r untouched
// Leaf: _Newton_Raphson@12
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3F60, 505 bytes (+ int3 pad to 507). libvorbis 1.0.1 lib/lsp.c static "for spit-and-polish
//   only" helper, inside libpk's statically linked Xiph decoder (0xC89xxx..0xCAxxxx, "Xiph.Org libVorbis I 20030909" @0x012BF240).
// Identity (proven): the only callers are 0x00CA44C9 and 0x00CA44D8 in vorbis_lpc_to_lsp (0x00CA4160, `__fastcall` lpc/lsp,
//   m on the stack: (m+1)>>1 / m>>1 orders, four alloca'd (order2+1)-float arrays, the g1[g1_order]=1.f / lpc[i-1]+lpc[m-i] and
//   lpc[i-1]-lpc[m-i] halves, cheby 0x00CA3A40 twice, Laguerre_With_Deflation 0x00CA3B80 twice with the `|| return -1`, then these
//   two calls, then qsort(.., 4, comp 0x00CA3B30) twice). The body is the lsp.c text verbatim: alloca(ord*sizeof(double)) through
//   __alloca_probe 0x00BFEA30, root[i]=r[i], `while(error>1e-20)` (1e-20 double @0x012BF380, 0.0 double @0x0122ED70), Horner
//   pp=pp*rooti+p / p=p*rooti+a[k], delta=p/pp, error+=delta*delta, `if(count>40)return(-1); count++;`, r[i]=root[i].
// Convention: VC7.1's private convention for a TU-local static with a same-TU caller (ord in esi; GOTCHAS chunk 585/586, the
//   landed cheby 0x00CA3A40 precedent); retail pops its two stack arguments (`ret 8`), so the static is `__stdcall` (GOTCHAS
//   0xC96320: an LTCG out-of-line STIN helper popping stack args is __stdcall). Plain cdecl gives the same body with `ret`.
// Label: the manifest "Math_SolveLinearSystemGaussian" is a descriptive placeholder (it is not a linear solve); the real name is
//   the lsp.c static `Newton_Raphson`. RELOCATION_MATCH on the second probe.
#include <malloc.h>
#define alloca _alloca
extern "C" {
static __declspec(noinline) int __stdcall Newton_Raphson(float *a,int ord,float *r){
  int i, k, count=0;
  double error=1.f;
  double *root=(double*)alloca(ord*sizeof(*root));

  for(i=0; i<ord;i++) root[i] = r[i];

  while(error>1e-20){
    error=0;

    for(i=0; i<ord; i++) { /* Update each point. */
      double pp=0.,delta;
      double rooti=root[i];
      double p=a[ord];
      for(k=ord-1; k>= 0; k--) {

	pp= pp* rooti + p;
	p = p * rooti + a[k];
      }

      delta = p/pp;
      root[i] -= delta;
      error+= delta*delta;
    }

    if(count>40)return(-1);

    count++;
  }

  for(i=0; i<ord;i++) r[i] = root[i];
  return(0);
}
// Same-TU caller (the two Newton_Raphson calls of lsp.c vorbis_lpc_to_lsp, 0x00CA44C9 / 0x00CA44D8), present so the static
// gets retail's private convention. Not graded.
int __cdecl Newton_Raphson_caller(float* a, int ord, float* r) { return Newton_Raphson(a, ord, r); }
}