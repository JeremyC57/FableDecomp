// Behaviour test for CTCDPhysicalObstruction::OnInitialActivate 0x007FD3C0: activation clears the life-time limit (LifeTimer = -1)
// and touches nothing else, so a later SetLifeTimeSeconds-style store can set a real limit.
#include <cstdio>
struct CTCDPhysicalObstruction {
    float StartPos[3], EndPos[3]; long LifeTimer; float Radius;
    void OnInitialActivate() { LifeTimer = -1; }
};
int main()
{
    CTCDPhysicalObstruction o = { { 1, 2, 3 }, { 4, 5, 6 }, 300, 2.5f };
    o.OnInitialActivate();
    if (o.LifeTimer != -1 || o.Radius != 2.5f || o.StartPos[2] != 3.0f || o.EndPos[0] != 4.0f) { std::printf("007fd3c0_TEST FAIL\n"); return 1; }
    std::printf("007fd3c0_TEST PASS\n");
    return 0;
}