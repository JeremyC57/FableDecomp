// Behaviour test for 0082fd90: body copied from src/0082fd90.cpp (CThingAICreature::WorldUpdateBulletTime, the catalog's propagated "CTCLook::UpdateHeadPos"), callees stubbed.
#include <cstdio>
#include <cstring>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { float s = Z * Z; s += Y * Y; s += X * X; return (float)sqrt(s); }
};
static int g_seq; static int g_actionAt, g_baseAt, g_cameraAt, g_posAt, g_animAt; static float g_animDist;
static C3DVector g_cameraPos, g_thingPos;
class CCamera { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual C3DVector GetPosition() const { return g_cameraPos; }
};
static CCamera g_camera;
class CThing { public:
    virtual void v00() {}
    const C3DVector& GetPosition() const { g_posAt = ++g_seq; return g_thingPos; }
    CCamera* GetCamera() const { g_cameraAt = ++g_seq; return &g_camera; }
    void WorldUpdateBulletTime() { g_baseAt = ++g_seq; }
};
class CThingCreatureBase : public CThing { public:
    void UpdateAction() { g_actionAt = ++g_seq; }
    void UpdateAnimation(float distanceToCamera) { g_animAt = ++g_seq; g_animDist = distanceToCamera; }
};
class CThingAICreature : public CThingCreatureBase { public:
    void WorldUpdateBulletTime();
};
void CThingAICreature::WorldUpdateBulletTime()
{
    UpdateAction();
    CThing::WorldUpdateBulletTime();
    CCamera* camera = GetCamera();
    const C3DVector& pos = GetPosition();
    UpdateAnimation((camera->GetPosition() - pos).GetLength());
}
int main()
{
    CThingAICreature c;
    // 1. distance 3-4-12 -> 13; calls in order action, base update, camera, position, animation
    g_seq = 0; g_cameraPos = C3DVector(4, 6, 15); g_thingPos = C3DVector(1, 2, 3);
    c.WorldUpdateBulletTime();
    CHECK(g_actionAt == 1); CHECK(g_baseAt == 2); CHECK(g_cameraAt == 3); CHECK(g_posAt == 4); CHECK(g_animAt == 5);
    CHECK(fabs(g_animDist - 13.0f) < 1e-5f);
    // 2. coincident: distance 0
    g_cameraPos = g_thingPos; c.WorldUpdateBulletTime(); CHECK(g_animDist == 0.0f);
    // 3. negative offsets square away: (-2, -3, -6) -> 7
    g_cameraPos = C3DVector(-1, -1, -3); c.WorldUpdateBulletTime(); CHECK(fabs(g_animDist - 7.0f) < 1e-5f);
    if (fails == 0) printf("0082fd90_TEST PASS\n");
    else printf("0082fd90_TEST FAIL (%d)\n", fails);
    return fails;
}