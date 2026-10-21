#include <cstdio>
// Behaviour test for CPSCLight::WriteBinary (0x00B18950): records the stream calls.
struct CRGBColour { unsigned char B, G, R, A; };
struct Stream {
    char kinds[64]; long vals[64]; int n;
    Stream() : n(0) {}
    void Put(char k, long v) { kinds[n] = k; vals[n] = v; ++n; }
    void WriteEBOOL(bool v) { Put('b', v ? 1 : 0); }
    void WriteLong(long v) { Put('l', v); }
    void WriteULong(unsigned long v) { Put('u', (long)v); }
    void WriteFloat(float v) { Put('f', (long)v); }
    void WriteColour(const CRGBColour& c) { Put('c', c.R); }
};
struct CPSCLight {
    unsigned long LightPositionParam;
    float LightLifeSecs, LightRespawnDelaySecs, LightStartTime, LightTimelineSecs;
    float LightStartRenderWorldRadius, LightEndRenderWorldRadius;
    long LightAttenuationFactor, LightFadeInEndInteger, LightFadeOutBeginInteger;
    float LightWorldRadiusFadeMinimum;
    CRGBColour LightColourFadeMinimum;
    bool LightWorldRadiusFadeEnable : 1;
    bool LightUseLifeSecs : 1;
    bool LightEnabled : 1;
    bool LightRespawns : 1;
    bool LightColourFadeEnable : 1;
    bool LightUseStartColour : 1;
    bool LightUseMidColour : 1;
    bool LightUseEndColour : 1;
    bool LightUseFadeColour : 1;
    bool LightUseTimelineSecs : 1;
    bool StartedTimer : 1;
    CRGBColour LightStartColour, LightMidColour, LightEndColour;
    void WriteBinary(Stream* out)
    {
        out->WriteULong(LightPositionParam);
        out->WriteFloat(LightLifeSecs);
        out->WriteFloat(LightRespawnDelaySecs);
        out->WriteFloat(LightStartTime);
        out->WriteFloat(LightTimelineSecs);
        out->WriteFloat(LightStartRenderWorldRadius);
        out->WriteFloat(LightEndRenderWorldRadius);
        out->WriteLong(LightAttenuationFactor);
        out->WriteLong(LightFadeInEndInteger);
        out->WriteLong(LightFadeOutBeginInteger);
        out->WriteFloat(LightWorldRadiusFadeMinimum);
        out->WriteColour(LightColourFadeMinimum);
        out->WriteEBOOL(LightWorldRadiusFadeEnable);
        out->WriteEBOOL(LightUseLifeSecs);
        out->WriteEBOOL(LightEnabled);
        out->WriteEBOOL(LightRespawns);
        out->WriteEBOOL(LightColourFadeEnable);
        out->WriteEBOOL(LightUseStartColour);
        out->WriteEBOOL(LightUseMidColour);
        out->WriteEBOOL(LightUseEndColour);
        out->WriteEBOOL(LightUseFadeColour);
        out->WriteEBOOL(LightUseTimelineSecs);
        out->WriteEBOOL(StartedTimer);
        out->WriteColour(LightStartColour);
        out->WriteColour(LightMidColour);
        out->WriteColour(LightEndColour);
    }
};
int main()
{
    CPSCLight l;
    l.LightPositionParam = 7; l.LightLifeSecs = 1; l.LightRespawnDelaySecs = 2; l.LightStartTime = 3;
    l.LightTimelineSecs = 4; l.LightStartRenderWorldRadius = 5; l.LightEndRenderWorldRadius = 6;
    l.LightAttenuationFactor = 8; l.LightFadeInEndInteger = 9; l.LightFadeOutBeginInteger = 10;
    l.LightWorldRadiusFadeMinimum = 11; l.LightColourFadeMinimum.R = 12;
    l.LightWorldRadiusFadeEnable = true; l.LightUseLifeSecs = false; l.LightEnabled = true; l.LightRespawns = false;
    l.LightColourFadeEnable = true; l.LightUseStartColour = false; l.LightUseMidColour = true; l.LightUseEndColour = true;
    l.LightUseFadeColour = false; l.LightUseTimelineSecs = true; l.StartedTimer = true;
    l.LightStartColour.R = 13; l.LightMidColour.R = 14; l.LightEndColour.R = 15;
    Stream s;
    l.WriteBinary(&s);
    const char* kinds = "ufffffflllfcbbbbbbbbbbbccc";
    long vals[] = { 7, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 13, 14, 15 };
    int bad = s.n != 26;
    for (int i = 0; i < 26 && !bad; ++i) bad |= s.kinds[i] != kinds[i] || s.vals[i] != vals[i];
    if (bad) { std::printf("00b18950_TEST FAIL\n"); return 1; }
    std::printf("00b18950_TEST PASS\n");
    return 0;
}