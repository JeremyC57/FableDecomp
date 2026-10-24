#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CBase {
    C3DVector lastPos; float lastWeight; int calls;
    void AddAttractorGoTo(const C3DVector& position, float weight) { lastPos = position; lastWeight = weight; ++calls; }
};
struct CStringAndCage : CBase {
    void SetSACPosition(const C3DVector& position) { AddAttractorGoTo(position, 1.0f); }
};
int main()
{
    CStringAndCage m; m.calls = 0; m.lastWeight = 0.0f;
    C3DVector p; p.X = 3.0f; p.Y = -1.0f; p.Z = 0.5f;
    m.SetSACPosition(p);
    if (m.calls != 1 || m.lastWeight != 1.0f || m.lastPos.X != 3.0f || m.lastPos.Z != 0.5f) { std::printf("00871e40_TEST FAIL\n"); return 1; }
    std::printf("00871e40_TEST PASS\n");
    return 0;
}