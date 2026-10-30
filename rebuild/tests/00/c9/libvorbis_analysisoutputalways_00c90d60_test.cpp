// Behaviour test for libvorbis _analysis_output_always (0x00C90D60): the "x y" lines written for each sample -- x is the
// bark band of (4000*j/n + .25), the time (j+off)/8000 or j; y is todB(v[j]) (-140 for zero) or v[j] -- rendered here
// into a string buffer instead of a file.
#include <cstdio>
#include <cstring>
#include <cmath>

#define toBARK(n)   (13.1f*atan(.00074f*(n))+2.24f*atan((n)*(n)*1.85e-8f)+1e-4f*(n))
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif

static float todB(const float* x)
{
    union { unsigned int i; float f; } ix;
    ix.f = *x;
    ix.i = ix.i & 0x7fffffff;
    return (float)(ix.i * 7.17711438e-7f - 764.6161886f);
}

static void dump(char* out, const float* v, int n, int bark, int dB, i64 off)
{
    out[0] = 0;
    for (int j = 0; j < n; j++) {
        char line[64];
        if (bark) { float b = (float)toBARK((4000.f * j / n) + .25); std::sprintf(line, "%f ", b); }
        else if (off != 0) std::sprintf(line, "%f ", (double)(j + off) / 8000.);
        else std::sprintf(line, "%f ", (double)j);
        std::strcat(out, line);
        if (dB) { float val; if (v[j] == 0.) val = -140.; else val = todB(v + j); std::sprintf(line, "%f\n", val); }
        else std::sprintf(line, "%f\n", v[j]);
        std::strcat(out, line);
    }
}

int main()
{
    int ok = 1;
    char out[512];
    float v[3] = { 0.0f, 1.0f, -2.0f };
    dump(out, v, 3, 0, 0, 0);
    ok &= std::strcmp(out, "0.000000 0.000000\n1.000000 1.000000\n2.000000 -2.000000\n") == 0;
    dump(out, v, 2, 0, 1, 8000);
    ok &= std::strncmp(out, "1.000000 -140.000000\n1.000125 ", 30) == 0;
    float one = 1.0f;
    ok &= std::fabs(todB(&one) - 0.0f) < 0.5f;     // 0 dB for unity
    float ten = 10.0f;
    ok &= std::fabs(todB(&ten) - 20.0f) < 0.5f;    // 20 dB per decade
    dump(out, v, 1, 1, 0, 0);
    ok &= std::strncmp(out, "0.00", 4) == 0;
    if (!ok) { std::printf("00c90d60_TEST FAIL\n"); return 1; }
    std::printf("00c90d60_TEST PASS\n");
    return 0;
}