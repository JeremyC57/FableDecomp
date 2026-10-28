#include <cstdio>
// Behaviour test for CCamera::SetFOV (0x00A0BED0).
class CCamera {
public:
    unsigned int FOVFlags; float HorizontalFOV; float VerticalFOV;
    void SetFOV(float h, float v)
    {
        HorizontalFOV = h;
        VerticalFOV = v;
        FOVFlags = 1;
        if (v > 0.0f)
            FOVFlags = 3;
    }
};
int main()
{
    CCamera c;
    c.SetFOV(1.5f, 0.75f);
    bool ok = c.FOVFlags == 3 && c.HorizontalFOV == 1.5f && c.VerticalFOV == 0.75f;
    c.SetFOV(1.0f, 0.0f); ok = ok && c.FOVFlags == 1;
    c.SetFOV(1.0f, -2.0f); ok = ok && c.FOVFlags == 1 && c.VerticalFOV == -2.0f;
    if (!ok) { std::printf("00a0bed0_TEST FAIL\n"); return 1; }
    std::printf("00a0bed0_TEST PASS\n");
    return 0;
}