// vorbis_lpc_to_lsp(float*,float*,int) -- Convert LPC coefficients into sorted line spectral pairs.
// Leaf: @vorbis_lpc_to_lsp@12
// Flags: /TC /Gr
#include <malloc.h>
#include <math.h>
#define alloca _alloca
__declspec(dllimport) void __cdecl qsort(void*,unsigned int,unsigned int,int(__cdecl*)(const void*,const void*));
static void __stdcall cheby(float *g, int ord) {
  int i, j;

  g[0] *= .5f;
  for(i=2; i<= ord; i++) {
    for(j=ord; j >= i; j--) {
      g[j-2] -= g[j];
      g[j] += g[j]; 
    }
  }
}

static int __cdecl comp(const void *a,const void *b){
  return (*(float *)a<*(float *)b)-(*(float *)a>*(float *)b);
}

/* Newton-Raphson-Maehly actually functioned as a decent root finder,
   but there are root sets for which it gets into limit cycles
   (exacerbated by zero suppression) and fails.  We can't afford to
   fail, even if the failure is 1 in 100,000,000, so we now use
   Laguerre and later polish with Newton-Raphson (which can then
   afford to fail) */

#define EPSILON 10e-7
static int __stdcall Laguerre_With_Deflation(float *a,int ord,float *r){
  int i,m;
  double lastdelta=0.f;
  double *defl=alloca(sizeof(*defl)*(ord+1));
  for(i=0;i<=ord;i++)defl[i]=a[i];

  for(m=ord;m>0;m--){
    double new=0.f,delta;

    /* iterate a root */
    while(1){
      double p=defl[m],pp=0.f,ppp=0.f,denom;
      
      /* eval the polynomial and its first two derivatives */
      for(i=m;i>0;i--){
	ppp = new*ppp + pp;
	pp  = new*pp  + p;
	p   = new*p   + defl[i-1];
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
      new   -= delta;

      if(delta<0.f)delta*=-1;

      if(fabs(delta/new)<10e-12)break; 
      lastdelta=delta;
    }

    r[m-1]=new;

    /* forward deflation */
    
    for(i=m;i>0;i--)
      defl[i-1]+=new*defl[i];
    defl++;

  }
  return(0);
}


/* for spit-and-polish only */
static int __stdcall Newton_Raphson(float *a,int ord,float *r){
  int i, k, count=0;
  double error=1.f;
  double *root=alloca(ord*sizeof(*root));

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

  /* Replaced the original bubble sort with a real sort.  With your
     help, we can eliminate the bubble sort in our lifetime. --Monty */

  for(i=0; i<ord;i++) r[i] = root[i];
  return(0);
}


/* Convert lpc coefficients to lsp coefficients */
int vorbis_lpc_to_lsp(float *lpc,float *lsp,int m){
  int order2=(m+1)>>1;
  int g1_order,g2_order;
  float *g1=alloca(sizeof(*g1)*(order2+1));
  float *g2=alloca(sizeof(*g2)*(order2+1));
  float *g1r=alloca(sizeof(*g1r)*(order2+1));
  float *g2r=alloca(sizeof(*g2r)*(order2+1));
  int i;

  /* even and odd are slightly different base cases */
  g1_order=(m+1)>>1;
  g2_order=(m)  >>1;

  /* Compute the lengths of the x polynomials. */
  /* Compute the first half of K & R F1 & F2 polynomials. */
  /* Compute half of the symmetric and antisymmetric polynomials. */
  /* Remove the roots at +1 and -1. */
  
  g1[g1_order] = 1.f;
  for(i=1;i<=g1_order;i++) g1[g1_order-i] = lpc[i-1]+lpc[m-i];
  g2[g2_order] = 1.f;
  for(i=1;i<=g2_order;i++) g2[g2_order-i] = lpc[i-1]-lpc[m-i];
  
  if(g1_order>g2_order){
    for(i=2; i<=g2_order;i++) g2[g2_order-i] += g2[g2_order-i+2];
  }else{
    for(i=1; i<=g1_order;i++) g1[g1_order-i] -= g1[g1_order-i+1];
    for(i=1; i<=g2_order;i++) g2[g2_order-i] += g2[g2_order-i+1];
  }

  /* Convert into polynomials in cos(alpha) */
  cheby(g1,g1_order);
  cheby(g2,g2_order);

  /* Find the roots of the 2 even polynomials.*/
  if(Laguerre_With_Deflation(g1,g1_order,g1r) ||
     Laguerre_With_Deflation(g2,g2_order,g2r))
    return(-1);

  Newton_Raphson(g1,g1_order,g1r); /* if it fails, it leaves g1r alone */
  Newton_Raphson(g2,g2_order,g2r); /* if it fails, it leaves g2r alone */

  qsort(g1r,g1_order,sizeof(*g1r),comp);
  qsort(g2r,g2_order,sizeof(*g2r),comp);

  for(i=0;i<g1_order;i++)
    lsp[i*2] = acos(g1r[i]);

  for(i=0;i<g2_order;i++)
    lsp[i*2+1] = acos(g2r[i]);
  return(0);
}
