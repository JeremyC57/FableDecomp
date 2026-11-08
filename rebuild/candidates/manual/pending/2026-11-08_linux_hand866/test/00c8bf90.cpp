// Behaviour test for libvorbis vorbisfile.c _ov_splice (retail 0x00C8BF90): a copy of the splice; with n1 > n2 the shorter
// length and the second window are used, shared channels blend d*w^2 + s*(1-w^2), extra channels of the new stream are
// windowed up from zero, and samples past n are untouched.
#include <cstdio>
#include <cmath>

static void _ov_splice(float **pcm,float **lappcm,
		       int n1, int n2,
		       int ch1, int ch2,
		       float *w1, float *w2){
  int i,j;
  float *w=w1;
  int n=n1;

  if(n1>n2){
    n=n2;
    w=w2;
  }

  /* splice */
  for(j=0;j<ch1 && j<ch2;j++){
    float *s=lappcm[j];
    float *d=pcm[j];

    for(i=0;i<n;i++){
      float wd=w[i]*w[i];
      float ws=1.-wd;
      d[i]=d[i]*wd + s[i]*ws;
    }
  }
  /* window from zero */
  for(;j<ch2;j++){
    float *d=pcm[j];
    for(i=0;i<n;i++){
      float wd=w[i]*w[i];
      d[i]=d[i]*wd;
    }
  }

}

int main()
{
    float d0[8], d1[8], d2[8], s0[8], s1[8], w1[8], w2[8];
    float *pcm[3] = { d0, d1, d2 };
    float *lap[2] = { s0, s1 };
    int i;
    for (i = 0; i < 8; i++) {
        d0[i] = 1.f + i; d1[i] = 10.f + i; d2[i] = 100.f + i;
        s0[i] = -1.f - i; s1[i] = -10.f - i;
        w1[i] = 0.9f;                 /* must not be used (n1 > n2) */
        w2[i] = 0.1f * i;
    }
    /* n1 = 8 > n2 = 6: use n = 6 and w2; ch1 = 2 old channels, ch2 = 3 new channels */
    _ov_splice(pcm, lap, 8, 6, 2, 3, w1, w2);
    for (i = 0; i < 6; i++) {
        float wd = w2[i] * w2[i], ws = 1.f - wd;
        float e0 = (1.f + i) * wd + (-1.f - i) * ws;
        float e1 = (10.f + i) * wd + (-10.f - i) * ws;
        float e2 = (100.f + i) * wd;
        if (std::fabs(d0[i] - e0) > 1e-4f || std::fabs(d1[i] - e1) > 1e-4f || std::fabs(d2[i] - e2) > 1e-4f) {
            std::printf("00c8bf90_TEST FAIL\n"); return 1;
        }
    }
    for (i = 6; i < 8; i++)
        if (d0[i] != 1.f + i || d1[i] != 10.f + i || d2[i] != 100.f + i) { std::printf("00c8bf90_TEST FAIL\n"); return 1; }
    /* n1 < n2: the first window and n1 */
    for (i = 0; i < 8; i++) { d0[i] = 2.f; s0[i] = 4.f; w1[i] = 0.5f; }
    _ov_splice(pcm, lap, 4, 8, 1, 1, w1, w2);
    for (i = 0; i < 4; i++) if (std::fabs(d0[i] - (2.f * 0.25f + 4.f * 0.75f)) > 1e-5f) { std::printf("00c8bf90_TEST FAIL\n"); return 1; }
    if (d0[4] != 2.f) { std::printf("00c8bf90_TEST FAIL\n"); return 1; }
    std::printf("00c8bf90_TEST PASS\n");
    return 0;
}
