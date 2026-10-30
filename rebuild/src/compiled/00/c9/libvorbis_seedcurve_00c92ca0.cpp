// libvorbis::seed_curve(float* seed, const float** curves, float amp, int oc, int n, int linesper, float dBoffset) -- libvorbis psy.c static: pick the tone-masking curve for amp's 10 dB level (0..7) and raise seed[] to amp + curve along it, linesper apart, from oc
// Leaf: _seed_curve@28
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C92CA0, 376 bytes, private convention: seed in edi, the other six arguments on the
//   stack, callee pops (`ret 18h`). libpk's statically linked Xiph libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909"),
//   lib/psy.c `static void seed_curve(...)`.
// Identity (proven, two sources): (1) the 1.0.1 psy.c body verbatim against the bytes: P_LEVEL_0 30.0 (double 0x01292B40),
//   the `.1f` factor (double 0.10000000149 at 0x01238B48 = (double)0.1f), max(choice,0) / min(choice,P_LEVELS-1 = 7),
//   posts[0] - EHMER_OFFSET (16.0f at 0x01238164), `linesper>>1`, the `seed[seedptr] < lin` raise, five __ftol2 0x00BFEA70
//   calls; (2) its only caller is seed_loop 0x00C92E20 (xref: the call at 0x00C92EF2), itself called from the landed
//   _vp_tonemask 0x00C93E30, whose TU already carries these psy.c statics. Manifest label "Graphics_ConvertFloatToFixed"
//   is wrong.
// Shape: two levers. (a) Same-TU statics: with seed_loop and _vp_tonemask in the TU, a `static __declspec(noinline)
//   __stdcall` seed_curve gets retail's seed-in-edi convention and `ret 18h` at plain /O2 (no /GL). (b) psy.c is C, so
//   VC7.1's <stdlib.h> defines min/max first (`(((a) > (b)) ? (a) : (b))`) and os.h's `#ifndef max` versions never apply:
//   the stdlib max(choice,0) gives retail's `setle ; dec ; and` and min(choice,7) its `cmp 7 ; jl` (the os.h forms give
//   setl / jle). The 4x unrolled loop is the compiler's.
// Land the leaf only: seed_loop / seed_chase / max_seeds / _vp_tonemask are context (0x00C93E30 landed separately).
#define P_BANDS 17
#define P_LEVELS 8
#define P_LEVEL_0 30.
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40
#define EHMER_OFFSET 16
#define NEGINF -9999.f
extern "C" void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
#define alloca _alloca
#define max(a,b)    (((a) > (b)) ? (a) : (b))   /* VC7.1 <stdlib.h> (C), defined before os.h's #ifndef max */
#define min(a,b)    (((a) < (b)) ? (a) : (b))

extern "C" {                               /* psy.c is C: C names for the statics and the API */

typedef struct vorbis_info_psy {
    int blockflag; float ath_adjatt; float ath_maxatt; float tone_masteratt[P_NOISECURVES];
    float tone_centerboost; float tone_decay; float tone_abs_limit; float toneatt[P_BANDS];
    int noisemaskp; float noisemaxsupp; float noisewindowlo; float noisewindowhi; int noisewindowlomin;
    int noisewindowhimin; int noisewindowfixed; float noiseoff[P_NOISECURVES][P_BANDS];
    float noisecompand[NOISE_COMPAND_LEVELS]; float max_curve_dB; int normal_channel_p; int normal_point_p;
    int normal_start; int normal_partition; double normal_thresh;
} vorbis_info_psy;
typedef struct vorbis_look_psy {
    int n; vorbis_info_psy* vi; float*** tonecurves; float** noiseoffset; float* ath; long* octave; long* bark;
    long firstoc; long shiftoc; int eighth_octave_lines; int total_octave_lines; long rate;
} vorbis_look_psy;

static __declspec(noinline) void __stdcall seed_curve(float *seed,
                       const float **curves,
                       float amp,
                       int oc, int n,
                       int linesper,float dBoffset){
  int i,post1;
  int seedptr;
  const float *posts,*curve;

  int choice=(int)((amp+dBoffset-P_LEVEL_0)*.1f);
  choice=max(choice,0);
  choice=min(choice,P_LEVELS-1);
  posts=curves[choice];
  curve=posts+2;
  post1=(int)posts[1];
  seedptr=(int)(oc+(posts[0]-EHMER_OFFSET)*linesper-(linesper>>1));

  for(i=(int)posts[0];i<post1;i++){
    if(seedptr>0){
      float lin=amp+curve[i];
      if(seed[seedptr]<lin)seed[seedptr]=lin;
    }
    seedptr+=linesper;
    if(seedptr>=n)break;
  }
}

static void __stdcall seed_loop(vorbis_look_psy *p,
                      const float ***curves,
                      const float *f,
                      const float *flr,
                      float *seed,
                      float specmax){
  vorbis_info_psy *vi=p->vi;
  long n=p->n,i;
  float dBoffset=vi->max_curve_dB-specmax;

  /* prime the working vector with peak values */

  for(i=0;i<n;i++){
    float max=f[i];
    long oc=p->octave[i];
    while(i+1<n && p->octave[i+1]==oc){
      i++;
      if(f[i]>max)max=f[i];
    }

    if(max+6.f>flr[i]){
      oc=oc>>p->shiftoc;

      if(oc>=P_BANDS)oc=P_BANDS-1;
      if(oc<0)oc=0;

      seed_curve(seed,
                 curves[oc],
                 max,
                 p->octave[i]-p->firstoc,
                 p->total_octave_lines,
                 p->eighth_octave_lines,
                 dBoffset);
    }
  }
}

static void seed_chase(float *seeds, int linesper, long n){
  long  *posstack=(long*)alloca(n*sizeof(*posstack));
  float *ampstack=(float*)alloca(n*sizeof(*ampstack));
  long   stack=0;
  long   pos=0;
  long   i;

  for(i=0;i<n;i++){
    if(stack<2){
      posstack[stack]=i;
      ampstack[stack++]=seeds[i];
    }else{
      while(1){
        if(seeds[i]<ampstack[stack-1]){
          posstack[stack]=i;
          ampstack[stack++]=seeds[i];
          break;
        }else{
          if(i<posstack[stack-1]+linesper){
            if(stack>1 && ampstack[stack-1]<=ampstack[stack-2] &&
               i<posstack[stack-2]+linesper){
              /* we completely overlap, making stack-1 irrelevant.  pop it */
              stack--;
              continue;
            }
          }
          posstack[stack]=i;
          ampstack[stack++]=seeds[i];
          break;

        }
      }
    }
  }

  /* the stack now contains only the positions that are relevant. Scan
     'em straight through */

  for(i=0;i<stack;i++){
    long endpos;
    if(i<stack-1 && ampstack[i+1]>ampstack[i]){
      endpos=posstack[i+1];
    }else{
      endpos=posstack[i]+linesper+1; /* +1 is important, else bin 0 is
                                        discarded in short frames */
    }
    if(endpos>n)endpos=n;
    for(;pos<endpos;pos++)
      seeds[pos]=ampstack[i];
  }
}

static void __stdcall max_seeds(vorbis_look_psy *p,
                      float *seed,
                      float *flr){
  long   n=p->total_octave_lines;
  int    linesper=p->eighth_octave_lines;
  long   linpos=0;
  long   pos;

  seed_chase(seed,linesper,n); /* for masking */

  pos=p->octave[0]-p->firstoc-(linesper>>1);

  while(linpos+1<p->n){
    float minV=seed[pos];
    long end=((p->octave[linpos]+p->octave[linpos+1])>>1)-p->firstoc;
    if(minV>p->vi->tone_abs_limit)minV=p->vi->tone_abs_limit;
    while(pos+1<=end){
      pos++;
      if((seed[pos]>NEGINF && seed[pos]<minV) || minV==NEGINF)
        minV=seed[pos];
    }

    end=pos+p->firstoc;
    for(;linpos<p->n && p->octave[linpos]<=end;linpos++)
      if(flr[linpos]<minV)flr[linpos]=minV;
  }

  {
    float minV=seed[p->total_octave_lines-1];
    for(;linpos<p->n;linpos++)
      if(flr[linpos]<minV)flr[linpos]=minV;
  }

}

/* Same-TU caller chain (_vp_tonemask 0x00C93E30 landed; seed_loop 0x00C92E20 grades RELOCATION_MATCH from this TU too):
   here only so seed_curve gets retail's private convention. */
void __fastcall _vp_tonemask(vorbis_look_psy *p,
                  float *logfft,
                  float *logmask,
                  float global_specmax,
                  float local_specmax){

  int i,n=p->n;

  float *seed=(float*)alloca(sizeof(*seed)*p->total_octave_lines);
  float att=local_specmax+p->vi->ath_adjatt;
  for(i=0;i<p->total_octave_lines;i++)seed[i]=NEGINF;

  /* set the ATH (floating below localmax, not global max by a
     specified att) */
  if(att<p->vi->ath_maxatt)att=p->vi->ath_maxatt;

  for(i=0;i<n;i++)
    logmask[i]=p->ath[i]+att;

  /* tone masking */
  seed_loop(p,(const float ***)p->tonecurves,logfft,logmask,seed,global_specmax);
  max_seeds(p,seed,logmask);

}

}   /* extern "C" */