#include <cstdio>
struct CScriptModuleTransitionBetweenPoints {
    float FOVOverride; float FOV;
    bool GetModuleFOV(float& fov) const { fov = FOV; return true; }
};
int main()
{
    CScriptModuleTransitionBetweenPoints m; m.FOVOverride = -1.0f; m.FOV = 0.375f;
    float f = 0.0f;
    if (!m.GetModuleFOV(f) || f != 0.375f) { std::printf("00854340_TEST FAIL\n"); return 1; }
    std::printf("00854340_TEST PASS\n");
    return 0;
}