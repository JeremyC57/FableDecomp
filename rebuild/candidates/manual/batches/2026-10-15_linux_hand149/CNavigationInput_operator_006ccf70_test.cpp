#include <cstdio>
#include <math.h>
struct V { float X, Y, Z; bool IsEqual(const V& o, float t) const { return fabs(X - o.X) <= t && fabs(Y - o.Y) <= t && fabs(Z - o.Z) <= t; } };
struct In {
    V Destination; float Proximity; int MovementType; float Speed;
    bool operator==(const In& o) const
    {
        if (MovementType == o.MovementType && fabs(Proximity - o.Proximity) < 0.0001f && Destination.IsEqual(o.Destination, 0.0001f)
            && (MovementType != 0 || fabs(Speed - o.Speed) < 0.0001f))
            return true;
        return false;
    }
};
int main()
{
    In a = { { 1, 2, 3 }, 0.5f, 0, 2.0f }, b = a;
    bool fail = !(a == b);
    b.Speed = 3.0f; if (a == b) fail = true;
    a.MovementType = b.MovementType = 1; if (!(a == b)) fail = true;
    b.Destination.Y = 2.00005f; if (!(a == b)) fail = true;
    b.Destination.Y = 2.1f; if (a == b) fail = true;
    b = a; b.Proximity = 0.6f; if (a == b) fail = true;
    b = a; b.MovementType = 2; if (a == b) fail = true;
    if (fail) { std::printf("006ccf70_TEST FAIL\n"); return 1; }
    std::printf("006ccf70_TEST PASS\n");
    return 0;
}
