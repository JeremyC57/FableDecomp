// Behaviour test for 0x00BC7F70 CEngineInternalPrimitiveLine::Update (model copy).
#include <cstdio>

struct V3 { float X, Y, Z; };
struct Col { unsigned char B, G, R, A; };
struct SLine { V3 Start, End; Col Colour; float Width; unsigned long BankIndex; long BlendMode, BlendOp; bool UseZBuffer; };
static int g_loads = 0, g_nullLoads = 0;
struct CFrame { void BeginLoading(unsigned long, unsigned long) { ++g_loads; } };
struct SRec { CFrame* m_pFrame; };
struct CBank {
    SRec* first; SRec* last;
    SRec* Get(unsigned long i) { return i < (unsigned long)(last - first) ? &first[i] : 0; }
    CFrame* GetFrame(unsigned long i) { return Get(i) ? Get(i)->m_pFrame : 0; }
};
static CBank g_bank;
static float g_fade = -1.0f;
struct CLine {
    unsigned long EnginePrimitiveFlags; unsigned char Alpha;
    V3 Start, End; Col Colour; float Width; unsigned long BankIndex, Frame; long BlendMode, BlendOp; bool UseZBuffer;
    void Update(const SLine& line, unsigned long flags)
    {
        EnginePrimitiveFlags = flags; Start = line.Start; End = line.End; Colour = line.Colour; Alpha = Colour.A;
        Width = line.Width; BankIndex = line.BankIndex; Frame = 0; BlendMode = line.BlendMode; BlendOp = line.BlendOp;
        UseZBuffer = line.UseZBuffer;
        if (BankIndex) { CFrame* f = g_bank.GetFrame(BankIndex); if (f) f->BeginLoading(0, 0); else ++g_nullLoads; }
        g_fade = 0.0f;
    }
};

int main()
{
    static CFrame frames[3]; SRec recs[3] = { { &frames[0] }, { &frames[1] }, { &frames[2] } };
    g_bank.first = recs; g_bank.last = recs + 3;
    SLine l; l.Start.X = 1; l.Start.Y = 2; l.Start.Z = 3; l.End.X = 4; l.End.Y = 5; l.End.Z = 6;
    l.Colour.B = 1; l.Colour.G = 2; l.Colour.R = 3; l.Colour.A = 0x80; l.Width = 2.5f; l.BankIndex = 2;
    l.BlendMode = 4; l.BlendOp = 1; l.UseZBuffer = true;
    CLine c; c.Frame = 7;
    c.Update(l, 0x40);
    if (c.EnginePrimitiveFlags != 0x40 || c.Alpha != 0x80 || c.End.Z != 6 || c.Frame != 0 || c.BankIndex != 2 ||
        !c.UseZBuffer || g_loads != 1 || g_fade != 0.0f) { std::printf("00bc7f70_TEST FAIL\n"); return 1; }
    l.BankIndex = 9; c.Update(l, 0);
    l.BankIndex = 0; c.Update(l, 0);
    if (g_loads != 1 || g_nullLoads != 1) { std::printf("00bc7f70_TEST FAIL\n"); return 1; }
    std::printf("00bc7f70_TEST PASS\n");
    return 0;
}
