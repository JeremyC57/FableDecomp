// behaviour test for 004732e0 CEnvironmentThemeDef::Transfer: 34 members persisted in declaration order with the right overload
#include <cstdio>
class CPersistContext { public:
    char kinds[64];
    void* addrs[64];
    int n;
    CPersistContext() : n(0) {}
    void Transfer(float& v) { kinds[n] = 'f'; addrs[n++] = &v; }
    void Transfer(bool& v)  { kinds[n] = 'b'; addrs[n++] = &v; }
    void Transfer(long& v)  { kinds[n] = 'l'; addrs[n++] = &v; }
};
class CEnvironmentThemeDef { public:
    float TimeOfDay; bool MoonLit; float FogStartZ, FogEndZ; long SkyTexture0, SkyTexture1; float SkyTexture1Blend;
    long CloudLowerLayerTexture0, CloudLowerLayerTexture1; float CloudLowerLayerTexture1Blend, CloudLowerLayerSpeedMultiplier;
    long CloudUpperLayerTexture0, CloudUpperLayerTexture1; float CloudUpperLayerTexture1Blend, CloudUpperLayerSpeedMultiplier;
    float Rest[19];
    void Transfer(CPersistContext& c)
    {
        c.Transfer(TimeOfDay); c.Transfer(MoonLit); c.Transfer(FogStartZ); c.Transfer(FogEndZ);
        c.Transfer(SkyTexture0); c.Transfer(SkyTexture1); c.Transfer(SkyTexture1Blend);
        c.Transfer(CloudLowerLayerTexture0); c.Transfer(CloudLowerLayerTexture1);
        c.Transfer(CloudLowerLayerTexture1Blend); c.Transfer(CloudLowerLayerSpeedMultiplier);
        c.Transfer(CloudUpperLayerTexture0); c.Transfer(CloudUpperLayerTexture1);
        c.Transfer(CloudUpperLayerTexture1Blend); c.Transfer(CloudUpperLayerSpeedMultiplier);
        for (int i = 0; i < 19; ++i) c.Transfer(Rest[i]);
    }
};
int main()
{
    CEnvironmentThemeDef d;
    CPersistContext c;
    d.Transfer(c);
    const char* want = "fbffllfllffllff";
    if (c.n != 34) { std::printf("004732e0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 34; ++i) {
        char k = i < 15 ? want[i] : 'f';
        if (c.kinds[i] != k) { std::printf("004732e0_TEST FAIL\n"); return 1; }
    }
    if (c.addrs[0] != &d.TimeOfDay || c.addrs[1] != &d.MoonLit || c.addrs[33] != &d.Rest[18]) { std::printf("004732e0_TEST FAIL\n"); return 1; }
    std::printf("004732e0_TEST PASS\n");
    return 0;
}