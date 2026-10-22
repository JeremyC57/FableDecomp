// Behaviour test for CPSCDecalRenderer::WriteBinary (0x00B1BA10): records the stream writes and checks the order
// and the bit-field values.
#include <cstdio>
struct Stream {
    char kind[64]; double val[64]; int n;
    Stream() : n(0) {}
    void put(char k, double v) { kind[n] = k; val[n] = v; n++; }
    void WriteEBOOL(bool v) { put('b', v ? 1 : 0); }
    void WriteLong(long v) { put('l', (double)v); }
    void WriteULong(unsigned long v) { put('u', (double)v); }
    void WriteFloat(float v) { put('f', v); }
    void WriteColour(const unsigned int& c) { put('c', (double)c); }
};
struct Decal {
    float DecalLifeSecs; int DecalBankIndex; unsigned int StartColour, MidColour, EndColour;
    unsigned int BlendMode : 3; unsigned int BlendOp : 3; unsigned int Flags : 10; unsigned int FadeInEndInteger : 10;
    unsigned int FadeOutBeginInteger : 10; unsigned int StartRenderSize : 11; unsigned int AlphaFadeMinimum : 7;
    unsigned int EndRenderSize : 11; unsigned int AnimationTimeSecs : 14; unsigned int SizeFadeMinimum : 7;
    unsigned int UseStartColour : 1; unsigned int UseMidColour : 1; unsigned int UseEndColour : 1;
    unsigned int FlickerBias : 8; unsigned int FlickerMinAlphaInteger : 10; unsigned int FlickerMinSizeInteger : 10;
    unsigned int FlickerSpeed : 12;
    unsigned char AlphaFadeEnable : 1; unsigned char SizeFadeEnable : 1; unsigned char ForceAnimationTime : 1;
    unsigned char FlickerEnable : 1; unsigned char PoolingEnable : 1; unsigned char PoolIncreaseAlphaEnable : 1;
    unsigned char PoolIncreaseSizeEnable : 1; unsigned char PoolIncreaseLifeEnable : 1;
    unsigned char PoolIncreaseFrameEnable : 1;
    float MaxPoolSize, MaxPoolAlpha, MaxPoolLife, PoolFrameIncreaseRate, StencilCubeWidth;
    void WriteBinary(Stream* out)
    {
        out->WriteFloat(DecalLifeSecs); out->WriteLong(DecalBankIndex);
        out->WriteColour(StartColour); out->WriteColour(MidColour); out->WriteColour(EndColour);
        out->WriteULong(BlendMode); out->WriteULong(BlendOp); out->WriteULong(Flags); out->WriteULong(FadeInEndInteger);
        out->WriteULong(FadeOutBeginInteger); out->WriteULong(StartRenderSize); out->WriteULong(AlphaFadeMinimum);
        out->WriteULong(EndRenderSize); out->WriteULong(AnimationTimeSecs); out->WriteULong(SizeFadeMinimum);
        out->WriteEBOOL(UseStartColour); out->WriteEBOOL(UseMidColour); out->WriteEBOOL(UseEndColour);
        out->WriteEBOOL(AlphaFadeEnable); out->WriteEBOOL(SizeFadeEnable); out->WriteEBOOL(ForceAnimationTime);
        out->WriteULong(FlickerMinAlphaInteger); out->WriteULong(FlickerMinSizeInteger);
        out->WriteULong(FlickerBias); out->WriteULong(FlickerSpeed);
        out->WriteEBOOL(FlickerEnable); out->WriteEBOOL(PoolingEnable); out->WriteEBOOL(PoolIncreaseAlphaEnable);
        out->WriteEBOOL(PoolIncreaseSizeEnable); out->WriteEBOOL(PoolIncreaseLifeEnable);
        out->WriteFloat(MaxPoolSize); out->WriteFloat(MaxPoolAlpha); out->WriteFloat(MaxPoolLife);
        out->WriteEBOOL(PoolIncreaseFrameEnable);
        out->WriteFloat(PoolFrameIncreaseRate); out->WriteFloat(StencilCubeWidth);
    }
};
int main()
{
    Decal d;
    d.DecalLifeSecs = 2.5f; d.DecalBankIndex = -3; d.StartColour = 1; d.MidColour = 2; d.EndColour = 3;
    d.BlendMode = 5; d.BlendOp = 2; d.Flags = 1000; d.FadeInEndInteger = 999; d.FadeOutBeginInteger = 512;
    d.StartRenderSize = 2047; d.AlphaFadeMinimum = 100; d.EndRenderSize = 7; d.AnimationTimeSecs = 16000;
    d.SizeFadeMinimum = 127; d.UseStartColour = 1; d.UseMidColour = 0; d.UseEndColour = 1; d.FlickerBias = 200;
    d.FlickerMinAlphaInteger = 11; d.FlickerMinSizeInteger = 22; d.FlickerSpeed = 4000;
    d.AlphaFadeEnable = 0; d.SizeFadeEnable = 1; d.ForceAnimationTime = 0; d.FlickerEnable = 1; d.PoolingEnable = 0;
    d.PoolIncreaseAlphaEnable = 1; d.PoolIncreaseSizeEnable = 0; d.PoolIncreaseLifeEnable = 1;
    d.PoolIncreaseFrameEnable = 1;
    d.MaxPoolSize = 1.0f; d.MaxPoolAlpha = 2.0f; d.MaxPoolLife = 3.0f; d.PoolFrameIncreaseRate = 4.0f;
    d.StencilCubeWidth = 5.0f;
    Stream s;
    d.WriteBinary(&s);
    static const double want[36] = { 2.5, -3, 1, 2, 3, 5, 2, 1000, 999, 512, 2047, 100, 7, 16000, 127, 1, 0, 1,
        0, 1, 0, 11, 22, 200, 4000, 1, 0, 1, 0, 1, 1.0, 2.0, 3.0, 1, 4.0, 5.0 };
    static const char kinds[] = "flcccuuuuuuuuuubbbbbbuuuubbbbbfffbff";
    bool ok = s.n == 36;
    for (int i = 0; ok && i < 36; ++i) ok = s.kind[i] == kinds[i] && s.val[i] == want[i];
    if (!ok) { std::printf("00b1ba10_TEST FAIL\n"); return 1; }
    std::printf("00b1ba10_TEST PASS\n");
    return 0;
}
