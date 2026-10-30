// Behaviour test for libvorbis psy.c seed_curve (0x00C92CA0): level choice (clamped 0..7) and the seed raise loop.
#include <cstdio>
#define P_LEVELS 8
#define P_LEVEL_0 30.
#define EHMER_OFFSET 16
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

static void seed_curve(float *seed, const float **curves, float amp, int oc, int n, int linesper, float dBoffset){
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

static int fail(int k) { std::printf("00c92ca0_TEST FAIL %d\n", k); return 1; }

int main()
{
    // eight curves; curve c has posts {16, 20} and values c*100 + i at index i (curve = posts + 2)
    static float store[8][2 + 64];
    const float* curves[8];
    int c, i;
    for (c = 0; c < 8; ++c) {
        store[c][0] = 16.0f; store[c][1] = 20.0f;
        for (i = 0; i < 64; ++i) store[c][2 + i] = (float)(c * 100 + i);
        curves[c] = store[c];
    }
    float seed[64];
    for (i = 0; i < 64; ++i) seed[i] = -9999.0f;
    // amp 55, dBoffset 0 -> choice (55-30)*0.1 = 2; seedptr = oc + 0*4 - 2 = 8; i = 16..19 at 8, 12, 16, 20
    seed_curve(seed, curves, 55.0f, 10, 64, 4, 0.0f);
    if (seed[8] != 55.0f + 216.0f || seed[12] != 55.0f + 217.0f) return fail(1);
    if (seed[16] != 55.0f + 218.0f || seed[20] != 55.0f + 219.0f) return fail(2);
    if (seed[24] != -9999.0f || seed[9] != -9999.0f) return fail(3);
    // lower amp never lowers a seed; clamp below 0 picks curve 0
    seed_curve(seed, curves, 0.0f, 10, 64, 4, 0.0f);
    if (seed[8] != 271.0f) return fail(4);
    // huge amp clamps to curve 7; n stops the walk after the first store (seedptr 8 then 12 >= n 12)
    float s2[16];
    for (i = 0; i < 16; ++i) s2[i] = -9999.0f;
    seed_curve(s2, curves, 500.0f, 10, 12, 4, 0.0f);
    if (s2[8] != 500.0f + 716.0f) return fail(5);
    for (i = 9; i < 16; ++i) if (s2[i] != -9999.0f) return fail(6);
    // seedptr <= 0 positions are skipped
    float s3[16];
    for (i = 0; i < 16; ++i) s3[i] = -9999.0f;
    seed_curve(s3, curves, 31.0f, 0, 16, 4, 0.0f);    // choice 0, seedptr -2, 2, 6, 10
    if (s3[2] != 31.0f + 17.0f || s3[6] != 31.0f + 18.0f || s3[10] != 31.0f + 19.0f) return fail(7);
    if (s3[0] != -9999.0f) return fail(8);
    std::printf("00c92ca0_TEST PASS\n");
    return 0;
}