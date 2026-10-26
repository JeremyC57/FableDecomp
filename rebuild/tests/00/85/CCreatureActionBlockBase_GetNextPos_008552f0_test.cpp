// Behaviour test for 0x008552F0 CCreatureAction_BlockBase::GetNextPos: pos + dir*0.2*exp(-100 t^2), t = 1 - (total-current)/total.
#include <cstdio>
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CThing { public: C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CBaseIntelligentPointer { CThing* PItem; CThing* GetPItem() const { return PItem; } };
class CCreatureAction_BlockBase { public:
    CBaseIntelligentPointer Owner; long TotalFrames, CurrentFrame; C3DVector MovementDirection;
    C3DVector GetNextPos()
    {
        float t = 1.0f - (float)(TotalFrames - CurrentFrame) / TotalFrames;
        C3DVector dir = MovementDirection * 0.2f;
        return Owner.GetPItem()->GetPosition() + dir * (float)exp(t * t * -100.0f);
    }
};
static bool near_(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    CThing th; th.pos = C3DVector(1, 2, 3);
    CCreatureAction_BlockBase b; b.Owner.PItem = &th; b.TotalFrames = 10; b.CurrentFrame = 10; b.MovementDirection = C3DVector(5, 10, -5);
    C3DVector p = b.GetNextPos();          // t = 1: factor exp(-100) ~ 0
    b.CurrentFrame = 0;
    C3DVector q = b.GetNextPos();          // t = 0: factor 1 -> pos + dir*0.2
    if (!near_(p.X, 1) || !near_(p.Y, 2) || !near_(p.Z, 3) || !near_(q.X, 2) || !near_(q.Y, 4) || !near_(q.Z, 2)) {
        std::printf("008552f0_TEST FAIL\n"); return 1;
    }
    std::printf("008552f0_TEST PASS\n");
    return 0;
}