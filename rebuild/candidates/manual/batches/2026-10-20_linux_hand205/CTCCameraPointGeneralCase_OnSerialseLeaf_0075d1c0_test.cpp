// Behaviour test for CTCCameraPointGeneralCase::OnSerialseLeaf (0x0075D1C0): eight transfers in member order with their defaults.
#include <cstdio>
#include <cstring>

static const char* g_names[8]; static float g_floatDefaults[8]; static int g_boolDefaults[8]; static void* g_values[8];
static int g_n = 0;
struct CPersistContext {
    void Transfer(const char* name, float& value, const float& d) { g_names[g_n] = name; g_floatDefaults[g_n] = d; g_boolDefaults[g_n] = -1; g_values[g_n++] = &value; value = d; }
    void Transfer(const char* name, bool& value, const bool& d) { g_names[g_n] = name; g_boolDefaults[g_n] = d; g_values[g_n++] = &value; value = d; }
};
class CTCCameraPointGeneralCase {
public:
    float CameraStringLength, CameraCageRadius, CameraHeightOffset;
    bool AllowRightStickZoom, AllowRightStickRotation, AllowZTarget, AutoGoBehind;
    float AutoGoBehindTime;
    void OnSerialseLeaf(CPersistContext& context)
    {
        context.Transfer("StringLength", CameraStringLength, 5.0f);
        context.Transfer("CageRadius", CameraCageRadius, 2.0f);
        context.Transfer("HeightOffset", CameraHeightOffset, 2.0f);
        context.Transfer("AllowRightStickZoom", AllowRightStickZoom, true);
        context.Transfer("AllowRightStickRotation", AllowRightStickRotation, true);
        context.Transfer("AllowZTarget", AllowZTarget, true);
        context.Transfer("AutoGoBehind", AutoGoBehind, true);
        context.Transfer("AutoGoBehindTime", AutoGoBehindTime, 3.0f);
    }
};

int main()
{
    CTCCameraPointGeneralCase c; CPersistContext ctx;
    std::memset(&c, 0, sizeof(c));
    c.OnSerialseLeaf(ctx);
    const char* expect[8] = { "StringLength", "CageRadius", "HeightOffset", "AllowRightStickZoom", "AllowRightStickRotation", "AllowZTarget", "AutoGoBehind", "AutoGoBehindTime" };
    bool ok = g_n == 8;
    for (int i = 0; ok && i < 8; ++i) ok = std::strcmp(g_names[i], expect[i]) == 0;
    ok = ok && g_values[0] == &c.CameraStringLength && g_values[7] == &c.AutoGoBehindTime && g_values[3] == &c.AllowRightStickZoom
        && c.CameraStringLength == 5.0f && c.CameraCageRadius == 2.0f && c.CameraHeightOffset == 2.0f && c.AutoGoBehindTime == 3.0f
        && c.AllowRightStickZoom && c.AllowRightStickRotation && c.AllowZTarget && c.AutoGoBehind;
    if (!ok) { std::printf("0075d1c0_TEST FAIL\n"); return 1; }
    std::printf("0075d1c0_TEST PASS\n");
    return 0;
}
