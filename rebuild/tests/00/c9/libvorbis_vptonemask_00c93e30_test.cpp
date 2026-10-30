// Behaviour test for 0x00C93E30 _vp_tonemask (libvorbis 1.0.1 psy.c) with its psy.c statics (seed_curve, seed_loop,
// seed_chase, max_seeds) copied verbatim; the alloca buffers become static arrays (host-independent).
#include <cstdio>
using std::printf;
static long  g_posstack[64];
static float g_ampstack[64];
static float g_seed[64];
#define P_BANDS 17
#define P_LEVELS 8
#define P_LEVEL_0 30.
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40
#define EHMER_OFFSET 16
#define NEGINF -9999.f
#define min(x,y)  ((x)>(y)?(y):(x))    /* os.h */
#define max(x,y)  ((x)<(y)?(y):(x))


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

static void seed_curve(float *seed,
                       const float **curves,
                       float amp,
                       int oc, int n,
                       int linesper,float dBoffset){
  int i,post1;
  int seedptr;
  const float *posts,*curve;

  int choice=(int)((amp+dBoffset-P_LEVEL_0)*.1);
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

static void seed_loop(vorbis_look_psy *p,
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
  long  *posstack=g_posstack;
  float *ampstack=g_ampstack;
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

static void max_seeds(vorbis_look_psy *p,
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

static void _vp_tonemask(vorbis_look_psy *p,
                  float *logfft,
                  float *logmask,
                  float global_specmax,
                  float local_specmax){

  int i,n=p->n;

  float *seed=g_seed;
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


static bool near_(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    static vorbis_info_psy vi;
    static float ath[4] = { 0, 0, 0, 0 };
    static long octave[4] = { 4, 6, 8, 10 };
    static float posts[20];
    static const float* levels[P_LEVELS];
    static const float** bands[P_BANDS];
    posts[0] = 16; posts[1] = 18; posts[2 + 16] = -10; posts[2 + 17] = -20;
    for (int l = 0; l < P_LEVELS; ++l) levels[l] = posts;
    for (int b = 0; b < P_BANDS; ++b) bands[b] = levels;
    vi.ath_adjatt = -50; vi.ath_maxatt = -40; vi.tone_abs_limit = 1000; vi.max_curve_dB = 100;
    vorbis_look_psy p;
    p.n = 4; p.vi = &vi; p.tonecurves = (float***)bands; p.ath = ath; p.octave = octave;
    p.firstoc = 0; p.shiftoc = 0; p.eighth_octave_lines = 2; p.total_octave_lines = 16;
    float logfft[4] = { -200, -200, -200, -200 };
    float logmask[4];
    _vp_tonemask(&p, logfft, logmask, 100, 0);              // nothing above the ATH: logmask = ath + max(att, maxatt)
    for (int i = 0; i < 4; ++i) if (!near_(logmask[i], -40)) { printf("00c93e30_TEST FAIL\n"); return 1; }
    vi.ath_adjatt = -30;                                     // att -30 above maxatt
    logfft[0] = 50;                                          // one tone seeds bins 3 (40) and 5 (30)
    _vp_tonemask(&p, logfft, logmask, 100, 0);
    // seed_chase spreads the two seeds over bins 4..7 (40, 40, 30, 30); max_seeds raises each line to its region minimum
    if (!near_(g_seed[4], 40) || !near_(g_seed[5], 40) || !near_(g_seed[6], 30) || !near_(g_seed[7], 30) || !near_(g_seed[8], -9999)) {
        printf("00c93e30_TEST FAIL\n"); return 1;
    }
    if (!near_(logmask[0], 40) || !near_(logmask[1], 30) || !near_(logmask[2], 30) || !near_(logmask[3], -30)) {
        printf("00c93e30_TEST FAIL\n"); return 1;
    }
    printf("00c93e30_TEST PASS\n");
    return 0;
}