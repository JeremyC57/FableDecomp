// Behaviour test for libvorbis psy.c bark_noise_hybridmp (retail 0x00C931D0): a copy of the weighted least-squares noise-floor
// fit; on a flat curve every windowed fit returns the input level (first pass, with offset, and the fixed-width second pass),
// a window set covering the whole band gives the fitted line, the `fixed` pass only lowers values, and the clamp at 0 holds.
#include <cstdio>
#include <cmath>
#include <cstdlib>

static void bark_noise_hybridmp(int n,const long *b,
                                const float *f,
                                float *noise,
                                const float offset,
                                const int fixed){

  float *N=(float*)malloc(n*sizeof(*N));
  float *X=(float*)malloc(n*sizeof(*N));
  float *XX=(float*)malloc(n*sizeof(*N));
  float *Y=(float*)malloc(n*sizeof(*N));
  float *XY=(float*)malloc(n*sizeof(*N));

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

  if (fixed <= 0) goto done;

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
 done:
  free(N); free(X); free(XX); free(Y); free(XY);
}

static int fail(const char* what) { std::printf("%s\n00c931d0_TEST FAIL\n", what); return 1; }

int main()
{
    enum { n = 32 };
    long b[n]; float f[n], noise[n], noise2[n]; int i;
    /* psy.c's window table: (lo-1)<<16 | (hi-1), lo negative at the start (mirrored tail), hi >= n at the end */
    for (i = 0; i < n; i++) { int lo = i - 4, hi = i + 5; b[i] = ((long)(lo - 1) << 16) + (hi - 1); }

    /* a flat curve fits itself (first pass with offset 140 as _vp_noisemask, then with the fixed window) */
    for (i = 0; i < n; i++) f[i] = 20.f;
    bark_noise_hybridmp(n, b, f, noise, 140.f, -1);
    for (i = 0; i < n; i++) if (std::fabs(noise[i] - 20.f) > 2e-3f) return fail("flat fit");
    bark_noise_hybridmp(n, b, f, noise, 0.f, 6);
    for (i = 0; i < n; i++) if (std::fabs(noise[i] - 20.f) > 2e-3f) return fail("flat fit, fixed pass");

    /* a line is reproduced by the sliding fit wherever the window is inside the band (the first windows mirror the band
       start, so they see an even extension of the line rather than the line itself) */
    for (i = 0; i < n; i++) f[i] = 10.f + 0.5f * i;
    bark_noise_hybridmp(n, b, f, noise, 100.f, -1);
    for (i = 5; i < n; i++) if (std::fabs(noise[i] - f[i]) > 5e-2f) return fail("line fit");

    /* the fixed pass only ever lowers the estimate */
    for (i = 0; i < n; i++) f[i] = (i % 7 == 3) ? 60.f : 5.f;
    bark_noise_hybridmp(n, b, f, noise, 0.f, -1);
    bark_noise_hybridmp(n, b, f, noise2, 0.f, 4);
    for (i = 0; i < n; i++) if (noise2[i] > noise[i] + 1e-4f) return fail("fixed pass lowers");

    /* very negative input: the clamp holds the fit at 0 - offset */
    for (i = 0; i < n; i++) f[i] = -300.f;
    bark_noise_hybridmp(n, b, f, noise, 140.f, -1);
    for (i = 0; i < n; i++) if (noise[i] < -140.f - 1e-3f) return fail("clamp");

    std::printf("00c931d0_TEST PASS\n");
    return 0;
}
