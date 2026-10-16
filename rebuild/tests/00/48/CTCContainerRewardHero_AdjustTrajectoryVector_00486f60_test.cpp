// behaviour test for 0x00486F60 (C3DVector fast normalise with the 1/sqrt mantissa table)
#include <cstdio>
#include <cmath>
#include <cstring>
static unsigned int g_table[128];
static void BuildTable()
{
    for (int idx = 0; idx < 128; ++idx) {
        int e = (idx >> 6) ? 127 : 128;
        double m = 1.0 + ((idx & 63) + 0.5) / 64.0;
        double x = std::ldexp(m, e - 127);
        double r = 1.0 / std::sqrt(x);
        int E = (380 - e) >> 1;
        double frac = r / std::ldexp(1.0, E - 127) - 1.0;
        int entry = (int)(frac * 256.0 + 0.5);
        if (entry < 0) entry = 0;
        if (entry > 255) entry = 255;
        g_table[idx] = (unsigned int)entry;
    }
}
struct V {
    float X, Y, Z;
    V& Normalise()
    {
        float x = X, y = Y, z = Z;
        float lsq = x * x + y * y + z * z;
        unsigned int bits; std::memcpy(&bits, &lsq, 4);
        unsigned int est = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (g_table[(bits >> 17) & 0x7f] << 15);
        float r; std::memcpy(&r, &est, 4);
        r = (3.0f - r * r * lsq) * r * 0.5f;
        X *= r; Y *= r; Z *= r;
        return *this;
    }
};
int main()
{
    BuildTable();
    float in[4][3] = { { 3, 0, 4 }, { 1, 1, 1 }, { 10, -20, 5 }, { 0.01f, 0.02f, 0.03f } };
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        V v = { in[i][0], in[i][1], in[i][2] };
        V& ref = v.Normalise();
        float len = std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
        if (&ref != &v || std::fabs(len - 1.0f) > 2e-3f) ok = false;
        if ((in[i][0] > 0) != (v.X > 0)) ok = false;
    }
    if (!ok) { std::printf("00486f60_TEST FAIL\n"); return 1; }
    std::printf("00486f60_TEST PASS\n");
    return 0;
}