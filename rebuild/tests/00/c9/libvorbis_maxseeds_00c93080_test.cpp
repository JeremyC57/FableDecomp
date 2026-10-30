// Behaviour test for libvorbis psy.c max_seeds (0x00C93080): after seed_chase, each floor bin is raised to the minimum
// seed of its octave span (clamped to tone_abs_limit; NEGINF seeds never lower the minimum), and the bins past the last
// span to the last seed. The alloca stacks of seed_chase are fixed arrays here.
#include <cstdio>
#define P_NOISECURVES 3
#define P_BANDS 17
#define NEGINF -9999.f
struct vorbis_info_psy {
    int blockflag; float ath_adjatt; float ath_maxatt; float tone_masteratt[P_NOISECURVES];
    float tone_centerboost; float tone_decay; float tone_abs_limit; float toneatt[P_BANDS];
};
struct vorbis_look_psy {
    int n; vorbis_info_psy* vi; float*** tonecurves; float** noiseoffset; float* ath; long* octave; long* bark;
    long firstoc; long shiftoc; int eighth_octave_lines; int total_octave_lines; long rate;
};
static void seed_chase(float *seeds, int linesper, long n){
  long  posstack[64];
  float ampstack[64];
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
  for(i=0;i<stack;i++){
    long endpos;
    if(i<stack-1 && ampstack[i+1]>ampstack[i]){
      endpos=posstack[i+1];
    }else{
      endpos=posstack[i]+linesper+1;
    }
    if(endpos>n)endpos=n;
    for(;pos<endpos;pos++)
      seeds[pos]=ampstack[i];
  }
}
static void max_seeds(vorbis_look_psy *p, float *seed, float *flr){
  long   n=p->total_octave_lines;
  int    linesper=p->eighth_octave_lines;
  long   linpos=0;
  long   pos;
  seed_chase(seed,linesper,n);
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
static int run(float seedValue, float limit, const float* expect)
{
    static long octave[4] = { 1, 3, 5, 7 };
    vorbis_info_psy vi; vi.tone_abs_limit = limit;
    vorbis_look_psy p; p.n = 4; p.vi = &vi; p.octave = octave; p.firstoc = 0; p.eighth_octave_lines = 2; p.total_octave_lines = 8;
    float seed[8]; for (int i = 0; i < 8; ++i) seed[i] = seedValue;
    float flr[4] = { 0.0f, 20.0f, 5.0f, 30.0f };
    max_seeds(&p, seed, flr);
    for (int k = 0; k < 4; ++k) if (flr[k] != expect[k]) return 0;
    return 1;
}
int main()
{
    static const float e1[4] = { 10.0f, 20.0f, 10.0f, 30.0f };   // floor raised to the seeds
    static const float e2[4] = { 5.0f, 20.0f, 5.0f, 30.0f };     // spans clamped to tone_abs_limit (the tail seed 10 is below 30)
    static const float e3[4] = { 0.0f, 20.0f, 5.0f, 30.0f };     // NEGINF seeds leave the floor alone
    if (!run(10.0f, 100.0f, e1) || !run(10.0f, 5.0f, e2) || !run(NEGINF, 100.0f, e3)) {
        std::printf("00c93080_TEST FAIL\n");
        return 1;
    }
    // a dip inside a span: the minimum seed of the span wins
    {
        static long octave[4] = { 1, 3, 5, 7 };
        vorbis_info_psy vi; vi.tone_abs_limit = 100.0f;
        vorbis_look_psy p; p.n = 4; p.vi = &vi; p.octave = octave; p.firstoc = 0; p.eighth_octave_lines = 2; p.total_octave_lines = 8;
        float seed[8] = { 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 10.0f };
        float flr[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        max_seeds(&p, seed, flr);
        if (flr[0] != 10.0f || flr[3] != 10.0f) { std::printf("00c93080_TEST FAIL\n"); return 1; }
    }
    std::printf("00c93080_TEST PASS\n");
    return 0;
}