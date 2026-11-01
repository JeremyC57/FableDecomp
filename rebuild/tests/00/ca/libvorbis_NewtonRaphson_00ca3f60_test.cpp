// Test for libvorbis Newton_Raphson (0x00CA3F60): polishes the real roots of a polynomial given as a[0..ord] (a[ord] leading)
// from approximate starting roots; returns 0 and writes the polished roots back, or returns -1 after 41 passes without
// convergence and leaves r untouched. (The test sizes the root buffer statically instead of alloca.)
#include <cstdio>
#include <cmath>
static int Newton_Raphson(float *a,int ord,float *r){
  int i, k, count=0;
  double error=1.f;
  double root[16];
  for(i=0; i<ord;i++) root[i] = r[i];
  while(error>1e-20){
    error=0;
    for(i=0; i<ord; i++) {
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
  for(i=0; i<ord;i++) r[i] = (float)root[i];
  return(0);
}
int main()
{
    bool ok = true;
    float a[3] = { -0.125f, -0.25f, 1.0f };          // (x - 0.5)(x + 0.25)
    float r[2] = { 0.45f, -0.2f };
    if (Newton_Raphson(a, 2, r) != 0) ok = false;
    if (std::fabs(r[0] - 0.5f) > 1e-6f || std::fabs(r[1] + 0.25f) > 1e-6f) ok = false;
    float b[3] = { 1.0f, 0.0f, 1.0f };               // x^2 + 1: no real roots
    float s[2] = { 0.3f, -0.7f };
    if (Newton_Raphson(b, 2, s) != -1) ok = false;
    if (s[0] != 0.3f || s[1] != -0.7f) ok = false;
    std::printf(ok ? "00ca3f60_TEST PASS\n" : "00ca3f60_TEST FAIL\n");
    return ok ? 0 : 1;
}