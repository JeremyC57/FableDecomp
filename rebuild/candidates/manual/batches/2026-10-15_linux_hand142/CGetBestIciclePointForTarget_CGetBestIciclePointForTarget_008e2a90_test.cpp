#include <cstdio>
// Host model of the CGetBestIciclePointForTarget constructor: TargetPos = pos + (pos - prev with Z = 0) * time.
struct Vec { float X, Y, Z; };
struct Functor {
    Vec TargetPos; float BestPosScore; Vec BestPos;
    Functor(const Vec& pos, const Vec& prev, float time) : BestPosScore(-1.0f)
    {
        TargetPos.X = pos.X + (pos.X - prev.X) * time;
        TargetPos.Y = pos.Y + (pos.Y - prev.Y) * time;
        TargetPos.Z = pos.Z + 0.0f * time;
    }
};
int main()
{
    Vec pos = { 10.0f, 20.0f, 3.0f };
    Vec prev = { 8.0f, 21.0f, 1.0f };
    Functor f(pos, prev, 2.0f);
    if (f.BestPosScore != -1.0f || f.TargetPos.X != 14.0f || f.TargetPos.Y != 18.0f || f.TargetPos.Z != 3.0f) { std::printf("008e2a90_TEST FAIL\n"); return 1; }
    std::printf("008e2a90_TEST PASS\n");
    return 0;
}
