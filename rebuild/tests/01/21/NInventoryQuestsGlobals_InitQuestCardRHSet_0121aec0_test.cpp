// Behaviour test for 0x0121AEC0 NInventoryQuestsGlobals' dynamic initializer: QuestCardRHSet's Up is the
// (0,0,-1) axis and its Forward is minus the (0,-1,0) axis
#include <stdio.h>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};
class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
namespace NInventoryQuestsGlobals {
const C3DVector NEGATIVE_Y_AXIS(0.0f, -1.0f, 0.0f);
const C3DVector NEGATIVE_Z_AXIS(0.0f, 0.0f, -1.0f);
CRightHandedSet QuestCardRHSet(NEGATIVE_Z_AXIS, -NEGATIVE_Y_AXIS);
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
static bool Is(const C3DVector& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }
int main()
{
    using NInventoryQuestsGlobals::QuestCardRHSet;
    bool ok = true;
    ok = ok && Is(QuestCardRHSet.Up, 0.0f, 0.0f, -1.0f);
    ok = ok && Is(QuestCardRHSet.Forward, 0.0f, 1.0f, 0.0f);
    CRightHandedSet local(NInventoryQuestsGlobals::NEGATIVE_Z_AXIS, -NInventoryQuestsGlobals::NEGATIVE_Y_AXIS);
    ok = ok && Is(local.Up, 0.0f, 0.0f, -1.0f) && Is(local.Forward, 0.0f, 1.0f, 0.0f);
    ok = ok && Is(NInventoryQuestsGlobals::NEGATIVE_Y_AXIS, 0.0f, -1.0f, 0.0f);   // the negation made a copy
    if (ok) { printf("0121aec0_TEST PASS\n"); return 0; }
    printf("0121aec0_TEST FAIL\n");
    return 1;
}