// Behaviour test for 0x00B20630: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model: an output stream that logs each write; the offset data's flags and two parallel vector arrays.
struct V3 { float X, Y, Z; };
struct Out { int n; unsigned int ulongs[4]; int bools[4]; int nb; int nu; V3 vecs[16]; int nv;
    Out() : n(0), nb(0), nu(0), nv(0) {}
    void WriteEBOOL(bool b) { bools[nb++] = b ? 1 : 0; } void WriteULONG(unsigned int v) { ulongs[nu++] = v; } void Write3DVector(const V3& v) { vecs[nv++] = v; } };
struct Data {
    bool OffsetOnlyAtStartup, ResetEachFrame; V3 limits[4]; V3 speeds[4]; int count;
    void WriteBinary(Out& out) const
    {
        out.WriteEBOOL(OffsetOnlyAtStartup);
        out.WriteEBOOL(ResetEachFrame);
        unsigned int c = (unsigned int)count;
        out.WriteULONG(c);
        for (unsigned int i = 0; i < c; ++i) { out.Write3DVector(limits[i]); out.Write3DVector(speeds[i]); }
    }
};
static void Run()
{
    Data d; d.OffsetOnlyAtStartup = true; d.ResetEachFrame = false; d.count = 2;
    for (int i = 0; i < 2; ++i) { d.limits[i].X = (float)i; d.limits[i].Y = 0; d.limits[i].Z = 0; d.speeds[i].X = 10.0f + i; d.speeds[i].Y = 0; d.speeds[i].Z = 0; }
    Out o; d.WriteBinary(o);
    CHECK(o.nb == 2 && o.bools[0] == 1 && o.bools[1] == 0 && o.nu == 1 && o.ulongs[0] == 2u && o.nv == 4);
    CHECK(o.vecs[0].X == 0.0f && o.vecs[1].X == 10.0f && o.vecs[2].X == 1.0f && o.vecs[3].X == 11.0f);
    d.count = 0; Out e; d.WriteBinary(e); CHECK(e.nu == 1 && e.ulongs[0] == 0u && e.nv == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00b20630_TEST FAIL\n"); return 1; }
    std::printf("00b20630_TEST PASS\n");
    return 0;
}
