// Behaviour test for 0x006b2b10 CEnvironment::OverrideWindDirection
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CEnvironment {
    float WindSpeed; C3DVector WindDirection; bool OverrideWindDirectionFlag; C3DVector OldWindDirection; float OldWindSpeed;
    void OverrideWindDirection(const C3DVector& direction, float speed)
    {
        if (!OverrideWindDirectionFlag) {
            OldWindDirection = WindDirection;
            OverrideWindDirectionFlag = true;
            OldWindSpeed = WindSpeed;
        }
        WindDirection = direction;
        if (speed >= 0.0f)
            WindSpeed = speed;
    }
};
int main()
{
    CEnvironment e; e.WindSpeed = 2.0f; e.WindDirection.x = 1; e.WindDirection.y = 0; e.WindDirection.z = 0;
    e.OverrideWindDirectionFlag = false; e.OldWindSpeed = 0; e.OldWindDirection.x = e.OldWindDirection.y = e.OldWindDirection.z = 0;
    C3DVector d = { 0, 1, 0 };
    e.OverrideWindDirection(d, 5.0f);
    bool ok = e.OverrideWindDirectionFlag && e.OldWindSpeed == 2.0f && e.OldWindDirection.x == 1 && e.WindDirection.y == 1 && e.WindSpeed == 5.0f;
    C3DVector d2 = { 0, 0, 1 };
    e.OverrideWindDirection(d2, -1.0f);   // second override keeps the saved values, negative speed is ignored
    ok = ok && e.OldWindSpeed == 2.0f && e.OldWindDirection.x == 1 && e.WindDirection.z == 1 && e.WindSpeed == 5.0f;
    if (!ok) { std::printf("006b2b10_TEST FAIL\n"); return 1; }
    std::printf("006b2b10_TEST PASS\n");
    return 0;
}
