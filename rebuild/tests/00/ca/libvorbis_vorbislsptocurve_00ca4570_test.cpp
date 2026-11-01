// Behaviour test for libvorbis vorbis_lsp_to_curve 0x00CA4570 (lsp.c, FLOAT_LOOKUP build): the lookup helpers are modelled by
// their exact functions (cos, 1/sqrt, 1/sqrt(2^e), 10^(x/20) clamped to 1 above 0 dB), and the curve is checked against the
// closed form q = fromdB(amp / sqrt((p+q) * 2^m) - ampoffset) with lsp[] turned into cosines and runs of equal map[] sharing q.
#include <cstdio>
#include <cmath>

#define M_PI_F (3.1415926536f)
static float vorbis_coslook(float a) { return (float)std::cos((double)a); }
static float vorbis_invsqlook(float a) { return (float)(1.0 / std::sqrt((double)a)); }
static float vorbis_invsq2explook(int a) { return (float)(1.0 / std::sqrt(std::pow(2.0, (double)a))); }
static float vorbis_fromdBlook(float a) { return a > 0.f ? 1.f : (float)std::exp((double)a * .11512925); }

static void vorbis_lsp_to_curve(float *curve,int *map,int n,int ln,float *lsp,int m,
                                float amp,float ampoffset){
  int i;
  float wdel=M_PI_F/ln;
  for(i=0;i<m;i++)lsp[i]=vorbis_coslook(lsp[i]);

  i=0;
  while(i<n){
    int k=map[i];
    int qexp;
    float p=.7071067812f;
    float q=.7071067812f;
    float w=vorbis_coslook(wdel*k);
    float *ftmp=lsp;
    int c=m>>1;

    do{
      q*=ftmp[0]-w;
      p*=ftmp[1]-w;
      ftmp+=2;
    }while(--c);

    if(m&1){
      q*=ftmp[0]-w;
      q*=q;
      p*=p*(1.f-w*w);
    }else{
      q*=q*(1.f+w);
      p*=p*(1.f-w);
    }

    q=(float)frexp(p+q,&qexp);
    q=vorbis_fromdBlook(amp*
                        vorbis_invsqlook(q)*
                        vorbis_invsq2explook(qexp+m)-
                        ampoffset);

    do{
      curve[i++]*=q;
    }while(map[i]==k);
  }
}

// the plain reference: same recurrence in double, without the frexp split
static double reference_q(const double* coslsp, int m, double w, double amp, double ampoffset)
{
    double p = .7071067812, q = .7071067812;
    int j;
    for (j = 0; j + 1 < m; j += 2) { q *= coslsp[j] - w; p *= coslsp[j + 1] - w; }
    if (m & 1) { q *= coslsp[j] - w; q *= q; p *= p * (1.0 - w * w); }
    else { q *= q * (1.0 + w); p *= p * (1.0 - w); }
    double x = amp / std::sqrt((p + q) * std::pow(2.0, (double)m)) - ampoffset;
    return x > 0.0 ? 1.0 : std::exp(x * .11512925);
}

static bool run(int m)
{
    const int n = 16, ln = 64;
    float lsp[8]; double coslsp[8];
    for (int j = 0; j < m; ++j) { lsp[j] = 0.3f + 0.35f * j; coslsp[j] = std::cos((double)lsp[j]); }
    int map[n + 1]; float curve[n];
    for (int i = 0; i < n; ++i) { map[i] = (i * 5) / 2; curve[i] = 2.0f; }   // runs of two equal entries
    map[n] = -1;                                                              // the terminating map[++i] read
    vorbis_lsp_to_curve(curve, map, n, ln, lsp, m, 12.0f, 0.5f);
    for (int j = 0; j < m; ++j)
        if (std::fabs(lsp[j] - coslsp[j]) > 1e-5) return false;
    for (int i = 0; i < n; ++i) {
        double w = std::cos(M_PI_F / ln * map[i]);
        double expect = 2.0 * reference_q(coslsp, m, w, 12.0, 0.5);
        if (std::fabs(curve[i] - expect) > 1e-3 * (1.0 + std::fabs(expect))) return false;
        if (i + 1 < n && map[i] == map[i + 1] && curve[i] != curve[i + 1]) return false;
    }
    return true;
}

int main()
{
    if (!run(4) || !run(5)) { std::printf("00ca4570_TEST FAIL\n"); return 1; }
    std::printf("00ca4570_TEST PASS\n");
    return 0;
}