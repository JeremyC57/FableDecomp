#include <cstdio>
struct CCamera { float HorizontalFOV; float GetHorizontalFOV() const { return HorizontalFOV; } };
struct CScriptModuleBase {
    bool has; float value;
    bool GetModuleFOV(float& fov) const { if (has) { fov = value; return true; } return false; }
    bool GetModuleRollAngle(float& angle) const { if (has) { angle = value; return true; } return false; }
};
template<class T> struct CCountedPointer { T* Data; void* Info; T* operator->() const { return Data; } };
struct CScriptedControl {
    CCountedPointer<CScriptModuleBase> PScriptModule; CCamera InitCamera;
    float GetDefault() const
    {
        float fov;
        if (PScriptModule->GetModuleFOV(fov))
            return fov;
        return InitCamera.GetHorizontalFOV();
    }
};
int main()
{
    CScriptModuleBase mod; mod.has = true; mod.value = 0.5f;
    CScriptedControl c; c.PScriptModule.Data = &mod; c.PScriptModule.Info = 0; c.InitCamera.HorizontalFOV = 1.25f;
    if (c.GetDefault() != 0.5f) { std::printf("0087ba30_TEST FAIL\n"); return 1; }
    mod.has = false;
    if (c.GetDefault() != 1.25f) { std::printf("0087ba30_TEST FAIL\n"); return 1; }
    std::printf("0087ba30_TEST PASS\n");
    return 0;
}