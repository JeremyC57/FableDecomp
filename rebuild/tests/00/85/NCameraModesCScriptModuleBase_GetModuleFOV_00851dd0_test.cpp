#include <cstdio>
struct CScriptModuleBase {
    long  CutOutOverride;
    float FOVOverride;
    bool GetModuleFOV(float& fov) const
    {
        if (FOVOverride > 0.0f) { fov = FOVOverride; return true; }
        return false;
    }
};
int main()
{
    CScriptModuleBase m; m.CutOutOverride = 0; m.FOVOverride = 0.75f;
    float f = 2.0f;
    if (!m.GetModuleFOV(f) || f != 0.75f) { std::printf("00851dd0_TEST FAIL\n"); return 1; }
    m.FOVOverride = 0.0f; f = 2.0f;
    if (m.GetModuleFOV(f) || f != 2.0f) { std::printf("00851dd0_TEST FAIL\n"); return 1; }
    m.FOVOverride = -1.0f;
    if (m.GetModuleFOV(f) || f != 2.0f) { std::printf("00851dd0_TEST FAIL\n"); return 1; }
    std::printf("00851dd0_TEST PASS\n");
    return 0;
}