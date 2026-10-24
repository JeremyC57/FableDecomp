// Behaviour test for 0x00546A50 NUISystem::CZoomingComponent::Update: runs the base update, then clears Panning once both position axes are within 1e-4 of the pan target
#include <cstdio>
#include <cmath>
static const float g_positionEpsilon = 1e-4f;
static int g_baseUpdates = 0;
struct C2DVector { float X; float Y; };
struct CChangingStateComponent {
    C2DVector Position;
    C2DVector TargetPosition;
    void Update(float time) { (void)time; ++g_baseUpdates; }
};
struct CZoomingComponent : CChangingStateComponent {
    bool Panning;
    void Update(float time);
};
void CZoomingComponent::Update(float time)
{
    CChangingStateComponent::Update(time);
    if ((float)std::fabs(TargetPosition.X - Position.X) <= g_positionEpsilon &&
        (float)std::fabs(TargetPosition.Y - Position.Y) <= g_positionEpsilon)
        Panning = false;
}
int main()
{
    CZoomingComponent z;
    z.Panning = true;
    z.Position.X = 0.0f; z.Position.Y = 0.0f; z.TargetPosition.X = 1.0f; z.TargetPosition.Y = 0.0f;
    z.Update(0.1f);
    if (!z.Panning || g_baseUpdates != 1) { std::printf("00546a50_TEST FAIL\n"); return 1; }
    z.TargetPosition.X = 0.00005f; z.TargetPosition.Y = 0.5f;
    z.Update(0.1f);
    if (!z.Panning) { std::printf("00546a50_TEST FAIL\n"); return 1; }
    z.TargetPosition.Y = -0.00005f;
    z.Update(0.1f);
    if (z.Panning || g_baseUpdates != 3) { std::printf("00546a50_TEST FAIL\n"); return 1; }
    std::printf("00546a50_TEST PASS\n");
    return 0;
}