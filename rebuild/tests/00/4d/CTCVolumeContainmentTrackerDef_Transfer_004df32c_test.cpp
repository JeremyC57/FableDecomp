// Behaviour test for 0x004DF32C CTCVolumeContainmentTrackerDef::Transfer(CPersistContext&): the context sees the four members
// in declaration order through the transfer of their own types (bool, bool, float, float) and can change them in place.
#include <cstdio>

static int g_order[8]; static int g_count = 0;

class CPersistContext {
public:
    int Loading;
    void Transfer(bool& v)  { g_order[g_count++] = 1; if (Loading) v = !v; }
    void Transfer(float& v) { g_order[g_count++] = 2; if (Loading) v *= 2.0f; }
};

class CTCVolumeContainmentTrackerDef {
public:
    virtual ~CTCVolumeContainmentTrackerDef() { }
    virtual void Transfer(CPersistContext& context);
    bool  UpdateEveryFrame;
    bool  CalculateBlendedLightingChannel;
    float LightingChannelSphereModelRadius;
    float LightingChannelSphereModelZOffset;
};

void CTCVolumeContainmentTrackerDef::Transfer(CPersistContext& context)
{
    context.Transfer(UpdateEveryFrame);
    context.Transfer(CalculateBlendedLightingChannel);
    context.Transfer(LightingChannelSphereModelRadius);
    context.Transfer(LightingChannelSphereModelZOffset);
}

int main()
{
    CTCVolumeContainmentTrackerDef d;
    d.UpdateEveryFrame = false; d.CalculateBlendedLightingChannel = true;
    d.LightingChannelSphereModelRadius = 1.5f; d.LightingChannelSphereModelZOffset = -2.0f;
    CPersistContext c; c.Loading = 1;
    d.Transfer(c);
    if (g_count != 4 || g_order[0] != 1 || g_order[1] != 1 || g_order[2] != 2 || g_order[3] != 2) { std::printf("004df32c_TEST FAIL\n"); return 1; }
    if (!d.UpdateEveryFrame || d.CalculateBlendedLightingChannel || d.LightingChannelSphereModelRadius != 3.0f || d.LightingChannelSphereModelZOffset != -4.0f) {
        std::printf("004df32c_TEST FAIL\n"); return 1;
    }
    std::printf("004df32c_TEST PASS\n");
    return 0;
}