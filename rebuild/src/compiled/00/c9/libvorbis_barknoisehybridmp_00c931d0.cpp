// libvorbis::bark_noise_hybridmp(int n, const long* b, const float* f, float* noise, const float offset, const int fixed) -- psy.c: the noise-floor median estimate -- accumulate running sums N / X / XX / Y / XY of the energy-weighted (f + offset)^2 spectrum (alloca'd per call), then for every bin fit the weighted line (A + x*B) / D over the bark window b[i] (hi:lo packed, mirrored at the low end), writing R - offset into noise; with fixed > 0 repeat over a fixed-width window and keep the smaller estimate
// Leaf: _bark_noise_hybridmp@24
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C931D0, 2386 bytes: libvorbis 1.0.1 lib/psy.c `static void bark_noise_hybridmp(...)` kept out of
//   line with all six arguments on the stack and callee-pop `ret 18h` (ebp frame for the five `_alloca` calls: `__alloca_probe`
//   0x00BFEA30 with eax = (n*4+3)&~3). No other callee. Caller: _vp_noisemask 0x00C93C40 (landed global_vpnoisemask_00c93c40.cpp,
//   which calls it twice -- offset 140 / fixed -1, then offset 0 / p->vi->noisewindowfixed -- and binds `_bark_noise_hybridmp@24`).
// Identity (two sources): (1) the body is psy.c bark_noise_hybridmp verbatim -- the y < 1 clamp, w = y*y*.5 for bin 0, the five
//   prefix-sum arrays, the three-phase window walk (lo < 0 mirrored: N[hi] + N[-lo]; interior: differences; tail: the last fit
//   extended), R = (A + x*B)/D clamped at 0, noise[i] = R - offset, then the same over the fixed window keeping the minimum; (2) psy.c
//   file order: max_seeds 0x00C93080 (landed) precedes it and _vp_remove_floor 0x00C93B30 / _vp_noisemask 0x00C93C40 (landed) follow,
//   inside libpk's libVorbis block ("Xiph.Org libVorbis I 20030909" at 0x012BF240). The catalog name "FFT_ComputeMagnitudes" is a
//   descriptive guess.
// Shape: the psy.c static beside its real caller (the chunk 585/586 lever: `extern "C" { static __declspec(noinline) __stdcall }`), with
//   the landed _vp_noisemask body as that caller (not graded here). Constants: 1.0f, .5f pool.
// Original libvorbis psy.c; BSD-style terms in reference COPYING.
#define P_BANDS 17
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40
extern "C" void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
#define alloca _alloca
typedef struct vorbis_info_psy{
  int   blockflag;

  float ath_adjatt;
  float ath_maxatt;

  float tone_masteratt[P_NOISECURVES];
  float tone_centerboost;
  float tone_decay;
  float tone_abs_limit;
  float toneatt[P_BANDS];

  int noisemaskp;
  float noisemaxsupp;
  float noisewindowlo;
  float noisewindowhi;
  int   noisewindowlomin;
  int   noisewindowhimin;
  int   noisewindowfixed;
  float noiseoff[P_NOISECURVES][P_BANDS];
  float noisecompand[NOISE_COMPAND_LEVELS];

  float max_curve_dB;

  int normal_channel_p;
  int normal_point_p;
  int normal_start;
  int normal_partition;
  double normal_thresh;
} vorbis_info_psy;
typedef struct {
  int n;
  struct vorbis_info_psy *vi;

  float ***tonecurves;
  float **noiseoffset;

  float *ath;
  long  *octave;             /* in n.ocshift format */
  long  *bark;

  long  firstoc;
  long  shiftoc;
  int   eighth_octave_lines; /* power of two, please */
  int   total_octave_lines;  
  long  rate; /* cache it */
} vorbis_look_psy;

extern "C" {                               /* psy.c is C: C names for the static and the API */
static __declspec(noinline) void __stdcall bark_noise_hybridmp(int n,const long *b,
                                const float *f,
                                float *noise,
                                const float offset,
                                const int fixed){
  
  float *N=(float*)alloca(n*sizeof(*N));
  float *X=(float*)alloca(n*sizeof(*N));
  float *XX=(float*)alloca(n*sizeof(*N));
  float *Y=(float*)alloca(n*sizeof(*N));
  float *XY=(float*)alloca(n*sizeof(*N));

  float tN, tX, tXX, tY, tXY;
  int i;

  int lo, hi;
  float R, A, B, D;
  float w, x, y;

  tN = tX = tXX = tY = tXY = 0.f;

  y = f[0] + offset;
  if (y < 1.f) y = 1.f;

  w = y * y * .5;
    
  tN += w;
  tX += w;
  tY += w * y;

  N[0] = tN;
  X[0] = tX;
  XX[0] = tXX;
  Y[0] = tY;
  XY[0] = tXY;

  for (i = 1, x = 1.f; i < n; i++, x += 1.f) {
    
    y = f[i] + offset;
    if (y < 1.f) y = 1.f;

    w = y * y;
    
    tN += w;
    tX += w * x;
    tXX += w * x * x;
    tY += w * y;
    tXY += w * x * y;

    N[i] = tN;
    X[i] = tX;
    XX[i] = tXX;
    Y[i] = tY;
    XY[i] = tXY;
  }
  
  for (i = 0, x = 0.f;; i++, x += 1.f) {
    
    lo = b[i] >> 16;
    if( lo>=0 ) break;
    hi = b[i] & 0xffff;
    
    tN = N[hi] + N[-lo];
    tX = X[hi] - X[-lo];
    tXX = XX[hi] + XX[-lo];
    tY = Y[hi] + Y[-lo];    
    tXY = XY[hi] - XY[-lo];
    
    A = tY * tXX - tX * tXY;
    B = tN * tXY - tX * tY;
    D = tN * tXX - tX * tX;
    R = (A + x * B) / D;
    if (R < 0.f)
      R = 0.f;
    
    noise[i] = R - offset;
  }
  
  for ( ;; i++, x += 1.f) {
    
    lo = b[i] >> 16;
    hi = b[i] & 0xffff;
    if(hi>=n)break;
    
    tN = N[hi] - N[lo];
    tX = X[hi] - X[lo];
    tXX = XX[hi] - XX[lo];
    tY = Y[hi] - Y[lo];
    tXY = XY[hi] - XY[lo];
    
    A = tY * tXX - tX * tXY;
    B = tN * tXY - tX * tY;
    D = tN * tXX - tX * tX;
    R = (A + x * B) / D;
    if (R < 0.f) R = 0.f;
    
    noise[i] = R - offset;
  }
  for ( ; i < n; i++, x += 1.f) {
    
    R = (A + x * B) / D;
    if (R < 0.f) R = 0.f;
    
    noise[i] = R - offset;
  }
  
  if (fixed <= 0) return;
  
  for (i = 0, x = 0.f;; i++, x += 1.f) {
    hi = i + fixed / 2;
    lo = hi - fixed;
    if(lo>=0)break;

    tN = N[hi] + N[-lo];
    tX = X[hi] - X[-lo];
    tXX = XX[hi] + XX[-lo];
    tY = Y[hi] + Y[-lo];
    tXY = XY[hi] - XY[-lo];
    
    
    A = tY * tXX - tX * tXY;
    B = tN * tXY - tX * tY;
    D = tN * tXX - tX * tX;
    R = (A + x * B) / D;

    if (R - offset < noise[i]) noise[i] = R - offset;
  }
  for ( ;; i++, x += 1.f) {
    
    hi = i + fixed / 2;
    lo = hi - fixed;
    if(hi>=n)break;
    
    tN = N[hi] - N[lo];
    tX = X[hi] - X[lo];
    tXX = XX[hi] - XX[lo];
    tY = Y[hi] - Y[lo];
    tXY = XY[hi] - XY[lo];
    
    A = tY * tXX - tX * tXY;
    B = tN * tXY - tX * tY;
    D = tN * tXX - tX * tX;
    R = (A + x * B) / D;
    
    if (R - offset < noise[i]) noise[i] = R - offset;
  }
  for ( ; i < n; i++, x += 1.f) {
    R = (A + x * B) / D;
    if (R - offset < noise[i]) noise[i] = R - offset;
  }
}

/* Same-TU caller: psy.c _vp_noisemask (retail 0x00C93C40, landed), present so the static gets retail's convention. Not graded here. */
void __fastcall _vp_noisemask(vorbis_look_psy *p,
		   float *logmdct, 
		   float *logmask){

  int i,n=p->n;
  float *work=(float*)alloca(n*sizeof(*work));

  bark_noise_hybridmp(n,p->bark,logmdct,logmask,
		      140.,-1);

  for(i=0;i<n;i++)work[i]=logmdct[i]-logmask[i];

  bark_noise_hybridmp(n,p->bark,work,logmask,0.,
		      p->vi->noisewindowfixed);

  for(i=0;i<n;i++)work[i]=logmdct[i]-work[i];
  
  for(i=0;i<n;i++){
    int dB=logmask[i]+.5;
    if(dB>=NOISE_COMPAND_LEVELS)dB=NOISE_COMPAND_LEVELS-1;
    if(dB<0)dB=0;
    logmask[i]= work[i]+p->vi->noisecompand[dB];
  }

}
}