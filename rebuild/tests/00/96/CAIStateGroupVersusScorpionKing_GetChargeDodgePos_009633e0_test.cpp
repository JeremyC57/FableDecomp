// Behaviour test for CAIStateGroup_VersusScorpionKing::GetChargeDodgePos (0x009633E0): same logic over mocks.
#include <cstdio>
#include <cmath>
struct V { float x, y, z; };
static V Make(float x, float y, float z) { V v; v.x = x; v.y = y; v.z = z; return v; }
static void Normalise(V& v) { float l = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); v.x /= l; v.y /= l; v.z /= l; }
static void RotateXY(V& v, float turns)   // adds turns to atan2(X, Y), keeps the XY length and Z
{
    double a = std::atan2((double)v.x, (double)v.y) + turns * 6.283185307179586;
    double l = std::sqrt((double)v.x * v.x + (double)v.y * v.y);
    v.x = (float)(l * std::sin(a)); v.y = (float)(l * std::cos(a));
}
static float Dot(const V& a, const V& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
struct CState {
    V myPos, myFacing, enemyFacing; bool enemyAlive;
    void GetChargeDodgePos(V& pos)
    {
        pos = myPos;
        if (enemyAlive) {
            V facing = enemyFacing; Normalise(facing);
            V left = facing; RotateXY(left, -0.25f);
            V right = facing; RotateXY(right, 0.25f);
            V& side = Dot(left, myFacing) > 0.0f ? left : right;
            pos.x += side.x * 10.0f; pos.y += side.y * 10.0f; pos.z += side.z * 10.0f;
        }
    }
};
int main()
{
    CState s; s.myPos = Make(1, 2, 3); s.enemyFacing = Make(0, 5, 0); s.enemyAlive = true;
    s.myFacing = Make(-1, 0, 0);
    V p; s.GetChargeDodgePos(p);
    bool ok = Near(p.x, -9.0f) && Near(p.y, 2.0f) && Near(p.z, 3.0f);
    s.myFacing = Make(1, 0, 0); s.GetChargeDodgePos(p);
    ok = ok && Near(p.x, 11.0f) && Near(p.y, 2.0f);
    s.enemyAlive = false; s.GetChargeDodgePos(p);
    ok = ok && Near(p.x, 1.0f) && Near(p.y, 2.0f) && Near(p.z, 3.0f);
    if (!ok) { std::printf("009633e0_TEST FAIL\n"); return 1; }
    std::printf("009633e0_TEST PASS\n");
    return 0;
}