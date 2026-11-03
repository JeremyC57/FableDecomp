// Behaviour test for NParticleEngine::CPSCSpline::ReadBinary (0x00B1B1B0): a standalone model of the read -- two one-byte EBOOLs
// (SplineBounce, ScaleSplineSpeed), the SplineAnimSpeed float, then the CPSCSplineBase part (0x00B22B40) last; ReverseSpline and
// SplineCurrentPosition are not touched.
#include <cstdio>
#include <cstring>
#include <vector>
struct Stream {
    std::vector<unsigned char> data; unsigned long pos;
    Stream() : pos(0) {}
    void Read(void* out, long size) { if (size <= 0 || pos + size > data.size()) return; memcpy(out, &data[pos], size); pos += size; }
    float ReadFloat() { float v = 0; Read(&v, 4); return v; }
    bool ReadEBOOL() { unsigned char v = 0; Read(&v, 1); return v != 0; }
};
struct Spline {
    float SplineCurrentPosition; bool ReverseSpline, SplineBounce, ScaleSplineSpeed; float SplineAnimSpeed;
    int baseCalls; unsigned long posAtBase;
    void SplineBaseReadBinary(Stream& in) { ++baseCalls; posAtBase = in.pos; }
    void ReadBinary(Stream& in)
    {
        SplineBounce = in.ReadEBOOL();
        ScaleSplineSpeed = in.ReadEBOOL();
        SplineAnimSpeed = in.ReadFloat();
        SplineBaseReadBinary(in);
    }
};
int main()
{
    Stream s; unsigned char head[2] = { 0, 7 }; float speed = 2.5f;
    s.data.insert(s.data.end(), head, head + 2); s.data.insert(s.data.end(), (unsigned char*)&speed, (unsigned char*)&speed + 4);
    Spline sp; memset(&sp, 0, sizeof(sp)); sp.ReverseSpline = true; sp.SplineCurrentPosition = 0.75f; sp.SplineBounce = true;
    sp.ReadBinary(s);
    bool ok = !sp.SplineBounce && sp.ScaleSplineSpeed && sp.SplineAnimSpeed == 2.5f && sp.ReverseSpline &&
              sp.SplineCurrentPosition == 0.75f && sp.baseCalls == 1 && sp.posAtBase == 6 && s.pos == 6;
    printf(ok ? "00b1b1b0_TEST PASS\n" : "00b1b1b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
