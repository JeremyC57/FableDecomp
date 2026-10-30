// libvorbis::todB(const float* x) -- scales.h: 20*log10(|x|) approximated from the IEEE bit pattern ((x's bits & 0x7fffffff) * 7.17711438e-7f - 764.6161886f)
// Shape (retry1): parked as "BLOCKED: retail passes the pointer in EAX"; this is libvorbis 1.0.1 scales.h's static todB (VORBIS_IEEE_FLOAT32),
//   one private copy per including .c file. As a same-TU `static __declspec(noinline)` with a same-TU caller VC7.1 /O2 gives it the
//   private convention itself (x in eax, plain `ret`). The body is the pre-union scales.h form (`ogg_int32_t i=*(ogg_int32_t *)x;`):
//   the 1.0.1 union form stores the float and the masked int to the same word (34 bytes).
// Leaf: @todB@4
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9EF70, 29 bytes (one of three private copies: 0x00C90D40, 0x00C9DD40, 0x00C9EF70).
//   Constants 7.17711438e-7f @ 0x012BF264, 764.6161886f @ 0x012BF260.
// Label correction: the parked CUnprovenLogScale_00c9ef70::ApproximateLevel is descriptive; the body is libvorbis todB.
// Original libvorbis scales.h; BSD-style terms in reference COPYING.
extern "C" {
typedef int ogg_int32_t;

static __declspec(noinline) float todB(const float *x){
  ogg_int32_t i=*(ogg_int32_t *)x;
  i=i&0x7fffffff;
  return (float)(i * 7.17711438e-7f -764.6161886f);
}

/* Same-TU caller (stand-in for the including file's users). Not graded. */
float todB_caller(const float *v,int n){
  float m=-9999.f;
  int i;
  for(i=0;i<n;i++){
    float d=todB(v+i);
    if(d>m)m=d;
  }
  return m;
}
}