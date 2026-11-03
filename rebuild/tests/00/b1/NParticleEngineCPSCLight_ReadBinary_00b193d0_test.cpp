// Behaviour test for NParticleEngine::CPSCLight::ReadBinary (0x00B193D0): a standalone model of the read -- a chunked input stream whose
// Read copies from the current chunk or falls back to the overflow path across a chunk boundary; the light reads its position
// parameter, six floats, three longs, a float, the fade-minimum colour, eleven one-byte EBOOLs (any non-zero byte is true) and the
// start / mid / end colours, in the order CPSCLight::WriteBinary 0x00B18950 writes them.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
typedef unsigned int u32;                                   // the engine's 4-byte ULONG / SLONG on every host
typedef int s32;
struct Stream {
    std::vector<unsigned char> data; u32 pos; u32 chunk;   // chunk: bytes per source chunk
    Stream() : pos(0), chunk(5) {}
    void Read(void* out, long size) {
        if (size <= 0 || pos + (u32)size > data.size()) return;
        u32 remaining = chunk - pos % chunk;
        if ((u32)size <= remaining) { memcpy(out, &data[pos], size); pos += size; }
        else { for (long i = 0; i < size; ++i) ((unsigned char*)out)[i] = data[pos + i]; pos += size; }   // overflow path
    }
    u32 ReadULONG() { u32 v = 0; Read(&v, 4); return v; }
    s32 ReadSLONG() { s32 v = 0; Read(&v, 4); return v; }
    float ReadFloat() { float v = 0; Read(&v, 4); return v; }
    bool ReadEBOOL() { unsigned char v = 0; Read(&v, 1); return v != 0; }
    u32 ReadColour() { return ReadULONG(); }
};
struct Writer {
    std::vector<unsigned char> d;
    void Put(const void* p, int n) { d.insert(d.end(), (const unsigned char*)p, (const unsigned char*)p + n); }
    void U(u32 v) { Put(&v, 4); } void F(float v) { Put(&v, 4); } void B(unsigned char v) { Put(&v, 1); }
};
struct Light {
    u32 PositionParam; float LifeSecs, RespawnDelaySecs, StartTime, TimelineSecs, StartRadius, EndRadius;
    s32 Atten, FadeIn, FadeOut; float RadiusFadeMin; u32 FadeMinColour;
    bool b[11]; u32 Start, Mid, End;
    void ReadBinary(Stream& in) {
        PositionParam = in.ReadULONG();
        LifeSecs = in.ReadFloat(); RespawnDelaySecs = in.ReadFloat(); StartTime = in.ReadFloat();
        TimelineSecs = in.ReadFloat(); StartRadius = in.ReadFloat(); EndRadius = in.ReadFloat();
        Atten = in.ReadSLONG(); FadeIn = in.ReadSLONG(); FadeOut = in.ReadSLONG();
        RadiusFadeMin = in.ReadFloat(); FadeMinColour = in.ReadColour();
        for (int i = 0; i < 11; ++i) b[i] = in.ReadEBOOL();
        Start = in.ReadColour(); Mid = in.ReadColour(); End = in.ReadColour();
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    Writer w;
    w.U(7); w.F(1.5f); w.F(2.5f); w.F(0.25f); w.F(9.0f); w.F(3.0f); w.F(12.0f);
    w.U((u32)-4); w.U(100); w.U(900); w.F(0.125f); w.U(0x11223344);
    unsigned char flags[11] = { 1, 0, 2, 0, 0xFF, 1, 0, 0, 1, 0, 1 };
    for (int i = 0; i < 11; ++i) w.B(flags[i]);
    w.U(0xAABBCCDD); w.U(0x01020304); w.U(0xFFFFFFFF);
    Stream s; s.data = w.d;
    Light l; memset(&l, 0, sizeof(l)); l.ReadBinary(s);
    bool ok = l.PositionParam == 7 && near(l.LifeSecs, 1.5f) && near(l.RespawnDelaySecs, 2.5f) && near(l.StartTime, 0.25f)
        && near(l.TimelineSecs, 9.0f) && near(l.StartRadius, 3.0f) && near(l.EndRadius, 12.0f)
        && l.Atten == -4 && l.FadeIn == 100 && l.FadeOut == 900 && near(l.RadiusFadeMin, 0.125f)
        && l.FadeMinColour == 0x11223344 && l.Start == 0xAABBCCDD && l.Mid == 0x01020304 && l.End == 0xFFFFFFFF
        && s.pos == s.data.size();
    for (int i = 0; i < 11; ++i) ok = ok && l.b[i] == (flags[i] != 0);
    // a value read across a chunk boundary (the overflow path) comes back whole: 5-byte chunks split most of the dwords above
    Stream t; t.chunk = 3; t.data = w.d; Light m; memset(&m, 0, sizeof(m)); m.ReadBinary(t);
    ok = ok && m.PositionParam == 7 && m.Atten == -4 && m.Start == 0xAABBCCDD && t.pos == t.data.size();
    printf(ok ? "00b193d0_TEST PASS\n" : "00b193d0_TEST FAIL\n");
    return ok ? 0 : 1;
}