// Behaviour test for 0x008F57F0 CCreatureAction_DragonSwoop copy ctor: base copied, the three swoop points copied.
#include <cstdio>

static int g_baseCopies = 0;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

class CCombatActionBase {
public:
    int Id;
    CCombatActionBase(int id) : Id(id) {}
    CCombatActionBase(const CCombatActionBase& other) : Id(other.Id) { ++g_baseCopies; }
    virtual ~CCombatActionBase() {}
    virtual int Kind() const { return 0; }
};

class CCreatureAction_DragonSwoop : public CCombatActionBase {
public:
    C3DVector SwoopPoints[3];
    CCreatureAction_DragonSwoop(int id) : CCombatActionBase(id) {}
    virtual int Kind() const { return 7; }
    CCombatActionBase* Clone() const { return new CCreatureAction_DragonSwoop(*this); }
};

int main()
{
    CCreatureAction_DragonSwoop a(42);
    for (int i = 0; i < 3; ++i) { a.SwoopPoints[i].X = 1.0f + i; a.SwoopPoints[i].Y = 10.0f * i; a.SwoopPoints[i].Z = -2.0f; }
    int fail = 0;
    CCombatActionBase* c = a.Clone();
    CCreatureAction_DragonSwoop* b = (CCreatureAction_DragonSwoop*)c;
    if (g_baseCopies != 1 || c->Kind() != 7 || c->Id != 42) fail = 1;
    for (int i = 0; i < 3; ++i) {
        float dx = b->SwoopPoints[i].X - (1.0f + i), dy = b->SwoopPoints[i].Y - 10.0f * i, dz = b->SwoopPoints[i].Z + 2.0f;
        if (dx > 0.001f || dx < -0.001f || dy > 0.001f || dy < -0.001f || dz > 0.001f || dz < -0.001f) fail = 1;
    }
    delete c;
    if (fail) { std::printf("008f57f0_TEST FAIL\n"); return 1; }
    std::printf("008f57f0_TEST PASS\n");
    return 0;
}