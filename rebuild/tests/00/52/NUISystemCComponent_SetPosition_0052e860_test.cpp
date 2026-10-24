#include <cstdio>
struct C2DVector { float X; float Y; };
struct CComponent {
    C2DVector Position, TargetPosition, InitialPosition;
    void SetPosition(const C2DVector& p) { TargetPosition = p; InitialPosition = p; Position = p; }
};
int main()
{
    CComponent c;
    C2DVector z0 = { 9.0f, 9.0f };
    c.Position = z0; c.TargetPosition = z0; c.InitialPosition = z0;
    C2DVector p = { -3.0f, 4.0f };
    c.SetPosition(p);
    if (c.Position.X != -3.0f || c.Position.Y != 4.0f || c.TargetPosition.X != -3.0f || c.TargetPosition.Y != 4.0f ||
        c.InitialPosition.X != -3.0f || c.InitialPosition.Y != 4.0f) { std::printf("0052e860_TEST FAIL\n"); return 1; }
    std::printf("0052e860_TEST PASS\n");
    return 0;
}