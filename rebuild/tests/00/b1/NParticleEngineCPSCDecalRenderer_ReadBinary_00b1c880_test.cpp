// Behaviour test for NParticleEngine::CPSCDecalRenderer::ReadBinary (0x00B1C880): a standalone model of the read -- a chunked input
// stream (Read copies within the current chunk or byte-wise across a chunk boundary), and the decal renderer's packed layout (the
// retail bit-field words +0x20..+0x30 and the flag bytes +0x34/+0x35): a float, a long, three colours, ten unsigned longs truncated
// into their bit fields, three EBOOLs into the +0x2c bits, three EBOOLs into the +0x34 bits, four more bit fields, five EBOOLs, three
// floats, an EBOOL and two floats, in the order CPSCDecalRenderer::WriteBinary 0x00B1BA10 writes them. Neighbouring fields of each
// packed word must survive every merge (a missing mask or a wrong shift breaks them).
#include <cstdio>
#include <cstring>
#include <vector>
typedef unsigned int u32;
typedef int s32;
struct Stream {
    std::vector<unsigned char> data; u32 pos; u32 chunk;
    Stream() : pos(0), chunk(5) {}
    void Read(void* out, long size) {
        if (size <= 0 || pos + (u32)size > data.size()) return;
        u32 remaining = chunk - pos % chunk;
        if ((u32)size <= remaining) { memcpy(out, &data[pos], size); pos += size; }
        else { for (long i = 0; i < size; ++i) ((unsigned char*)out)[i] = data[pos + i]; pos += size; }
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
struct Decal {
    float DecalLifeSecs; s32 DecalBankIndex; u32 StartColour, MidColour, EndColour;
    u32 BlendMode : 3; u32 BlendOp : 3; u32 Flags : 10; u32 FadeInEndInteger : 10;
    u32 FadeOutBeginInteger : 10; u32 StartRenderSize : 11; u32 AlphaFadeMinimum : 7;
    u32 EndRenderSize : 11; u32 AnimationTimeSecs : 14; u32 SizeFadeMinimum : 7;
    u32 UseStartColour : 1; u32 UseMidColour : 1; u32 UseEndColour : 1; u32 FlickerBias : 8;
    u32 FlickerMinAlphaInteger : 10; u32 FlickerMinSizeInteger : 10;
    u32 FlickerSpeed : 12;
    unsigned char AlphaFadeEnable : 1; unsigned char SizeFadeEnable : 1; unsigned char ForceAnimationTime : 1;
    unsigned char FlickerEnable : 1; unsigned char PoolingEnable : 1; unsigned char PoolIncreaseAlphaEnable : 1;
    unsigned char PoolIncreaseSizeEnable : 1; unsigned char PoolIncreaseLifeEnable : 1; unsigned char PoolIncreaseFrameEnable : 1;
    float MaxPoolSize, MaxPoolAlpha, MaxPoolLife, PoolFrameIncreaseRate, StencilCubeWidth;
    void ReadBinary(Stream& in) {
        DecalLifeSecs = in.ReadFloat(); DecalBankIndex = in.ReadSLONG();
        StartColour = in.ReadColour(); MidColour = in.ReadColour(); EndColour = in.ReadColour();
        BlendMode = in.ReadULONG(); BlendOp = in.ReadULONG(); Flags = in.ReadULONG();
        FadeInEndInteger = in.ReadULONG(); FadeOutBeginInteger = in.ReadULONG(); StartRenderSize = in.ReadULONG();
        AlphaFadeMinimum = in.ReadULONG(); EndRenderSize = in.ReadULONG(); AnimationTimeSecs = in.ReadULONG();
        SizeFadeMinimum = in.ReadULONG();
        UseStartColour = in.ReadEBOOL(); UseMidColour = in.ReadEBOOL(); UseEndColour = in.ReadEBOOL();
        AlphaFadeEnable = in.ReadEBOOL(); SizeFadeEnable = in.ReadEBOOL(); ForceAnimationTime = in.ReadEBOOL();
        FlickerMinAlphaInteger = in.ReadULONG(); FlickerMinSizeInteger = in.ReadULONG();
        FlickerBias = in.ReadULONG(); FlickerSpeed = in.ReadULONG();
        FlickerEnable = in.ReadEBOOL(); PoolingEnable = in.ReadEBOOL(); PoolIncreaseAlphaEnable = in.ReadEBOOL();
        PoolIncreaseSizeEnable = in.ReadEBOOL(); PoolIncreaseLifeEnable = in.ReadEBOOL();
        MaxPoolSize = in.ReadFloat(); MaxPoolAlpha = in.ReadFloat(); MaxPoolLife = in.ReadFloat();
        PoolIncreaseFrameEnable = in.ReadEBOOL();
        PoolFrameIncreaseRate = in.ReadFloat(); StencilCubeWidth = in.ReadFloat();
    }
};
static Writer Make(u32 hi)   // hi: values wider than each field, so truncation is visible
{
    Writer w;
    w.F(4.5f); w.U((u32)-3); w.U(0xAABBCCDD); w.U(0x01020304); w.U(0x80808080);
    w.U(5 | hi); w.U(6 | hi); w.U(0x2AB | hi); w.U(0x155 | hi);          // BlendMode BlendOp Flags FadeInEnd
    w.U(0x3FF | hi); w.U(0x5A5 | hi); w.U(0x55 | hi);                    // FadeOutBegin StartRenderSize AlphaFadeMinimum
    w.U(0x7FE | hi); w.U(0x2AAA | hi); w.U(0x7F | hi);                   // EndRenderSize AnimationTimeSecs SizeFadeMinimum
    w.B(1); w.B(0); w.B(2);                                              // UseStart / Mid / End colour
    w.B(0); w.B(0xFF); w.B(1);                                           // AlphaFade SizeFade ForceAnimationTime
    w.U(0x201 | hi); w.U(0x1FE | hi); w.U(0xC3 | hi); w.U(0xABC | hi);  // FlickerMinAlpha FlickerMinSize FlickerBias FlickerSpeed
    w.B(1); w.B(0); w.B(1); w.B(1); w.B(0);                              // Flicker Pooling PoolAlpha PoolSize PoolLife
    w.F(2.0f); w.F(0.75f); w.F(30.0f); w.B(7); w.F(0.5f); w.F(3.25f);
    return w;
}
static bool Check(const Decal& d, const Stream& s)
{
    return d.DecalLifeSecs == 4.5f && d.DecalBankIndex == -3 && d.StartColour == 0xAABBCCDD && d.MidColour == 0x01020304
        && d.EndColour == 0x80808080 && d.BlendMode == 5 && d.BlendOp == 6 && d.Flags == 0x2AB && d.FadeInEndInteger == 0x155
        && d.FadeOutBeginInteger == 0x3FF && d.StartRenderSize == 0x5A5 && d.AlphaFadeMinimum == 0x55 && d.EndRenderSize == 0x7FE
        && d.AnimationTimeSecs == 0x2AAA && d.SizeFadeMinimum == 0x7F && d.UseStartColour == 1 && d.UseMidColour == 0
        && d.UseEndColour == 1 && d.AlphaFadeEnable == 0 && d.SizeFadeEnable == 1 && d.ForceAnimationTime == 1
        && d.FlickerMinAlphaInteger == 0x201 && d.FlickerMinSizeInteger == 0x1FE && d.FlickerBias == 0xC3 && d.FlickerSpeed == 0xABC
        && d.FlickerEnable == 1 && d.PoolingEnable == 0 && d.PoolIncreaseAlphaEnable == 1 && d.PoolIncreaseSizeEnable == 1
        && d.PoolIncreaseLifeEnable == 0 && d.MaxPoolSize == 2.0f && d.MaxPoolAlpha == 0.75f && d.MaxPoolLife == 30.0f
        && d.PoolIncreaseFrameEnable == 1 && d.PoolFrameIncreaseRate == 0.5f && d.StencilCubeWidth == 3.25f && s.pos == s.data.size();
}
int main()
{
    bool ok = true;
    for (int pass = 0; pass < 4; ++pass) {
        Writer w = Make(pass & 1 ? 0xFFFF0000u : 0u);
        Stream s; s.data = w.d; s.chunk = pass & 2 ? 3 : 64;
        Decal d; memset(&d, pass & 1 ? 0xFF : 0x00, sizeof(d));    // stale bits in every packed word must be overwritten
        d.ReadBinary(s);
        ok = ok && Check(d, s);
    }
    printf(ok ? "00b1c880_TEST PASS\n" : "00b1c880_TEST FAIL\n");
    return ok ? 0 : 1;
}