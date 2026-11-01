// libvorbis::Newton_Raphson(float* a, int ord, float* r) -- probe
// Leaf: _Newton_Raphson@12
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
int __cdecl Newton_Raphson_caller(float* a, int ord, float* r) { return Newton_Raphson(a, ord, r); }
}
