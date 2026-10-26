// Behaviour test for 006afcb0: body copied from src/006afcb0.cpp (CTCPhysicsBase::GetMapwhoPosition); the engine header replaced by a
// vtable-compatible model (28 leading virtual slots, Position at +0x0c) and the slot-28 offset virtual overridden by the test.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool feq(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-5f; }
class C2DVector { public:
    float X, Y;
    C2DVector() { }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector OffsetBy2D(const C2DVector& o) const { C3DVector result; result.X = X + o.X; result.Y = Y + o.Y; result.Z = Z; return result; }
};
static int g_offsetCalls = 0;
class CTCPhysicsBase { public:                        // test model of engine/CTCPhysicsBase.h: vptr, two words, Position at +0x0c
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {}
    virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {}
    virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {}
    virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {}
    virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {}
    virtual C2DVector GetOffset2D() { ++g_offsetCalls; C2DVector o; o.X = 0.0f; o.Y = 0.0f; return o; }
    long _04; long _08;
    float Position_X, Position_Y, Position_Z;
};
struct CTCPhysicsBaseVirtuals {                    // vtable-shaped view of `this` (slot = offset / 4)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual C2DVector GetOffset2D();   // slot 28 (+0x70)
};
struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    C3DVector GetMapwhoPosition();
};
C3DVector CTCPhysicsBase_Methods::GetMapwhoPosition()
{
    CTCPhysicsBaseVirtuals* self = reinterpret_cast<CTCPhysicsBaseVirtuals*>(this);
    return reinterpret_cast<const C3DVector*>(&Position_X)->OffsetBy2D(self->GetOffset2D());
}
struct CTestStandard : CTCPhysicsBase_Methods {    // the CTCPhysicsStandard-style override
    float OX, OY;
    virtual C2DVector GetOffset2D() { ++g_offsetCalls; C2DVector o; o.X = OX; o.Y = OY; return o; }
};
int main()
{
    CHECK((char*)&((CTCPhysicsBase*)0)->Position_X == (char*)0 + 0x0c);
    CTCPhysicsBase_Methods base; base.Position_X = 1.0f; base.Position_Y = 2.0f; base.Position_Z = 3.0f;
    C3DVector r = base.GetMapwhoPosition();
    CHECK(g_offsetCalls == 1); CHECK(feq(r.X, 1.0f) && feq(r.Y, 2.0f) && feq(r.Z, 3.0f));
    CTestStandard std; std.Position_X = 1.0f; std.Position_Y = 2.0f; std.Position_Z = 3.0f; std.OX = 0.5f; std.OY = -4.0f;
    r = std.GetMapwhoPosition();
    CHECK(g_offsetCalls == 2); CHECK(feq(r.X, 1.5f) && feq(r.Y, -2.0f) && feq(r.Z, 3.0f));
    CHECK(feq(std.Position_X, 1.0f) && feq(std.Position_Y, 2.0f));   // the position itself is untouched
    if (fails == 0) printf("006afcb0_TEST PASS\n"); else printf("006afcb0_TEST FAIL\n");
    return fails;
}