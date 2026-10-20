// Behaviour test for 0075c530: body copied from src/0075c530.cpp (CTCCameraPointDefinitionBase::IsAngleValid), world / player / camera callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C3DVector {
    float X, Y, Z;
};
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
static C3DVector g_heroPos; static int g_heroPosCalls;
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Flags91;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPosition() const { ++g_heroPosCalls; return g_heroPos; }
};
static CThing* g_hero;
struct CPlayer {
    CThing* GetControlledCreature() { return g_hero; }
};
static C3DVector g_cameraPos; static int g_cameraCalls;
struct CGameCameraManager {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {}
    virtual C3DVector PeekCurrentPos() const { ++g_cameraCalls; return g_cameraPos; }
};
static CGameCameraManager g_cameraManager; static CPlayer g_player;
struct CWorld {
    unsigned char _pad00[0x34];
    CGameCameraManager* PCameraManager;
    CGameCameraManager* GetCameraManager() const { return PCameraManager; }
    CPlayer* GetCurrentPlayer() const { return &g_player; }
};
static CWorld g_world;
inline C2DVector DeltaXY(const C3DVector& a, const C3DVector& b)
{
    C2DVector d;
    d.X = a.X - b.X;
    d.Y = a.Y - b.Y;
    return d;
}
static C3DVector g_activePos; static int g_activeCalls;
class CTCCameraPointDefinitionBase {
public:
#define VF(n) virtual void v##n() {}
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30) VF(31)
    VF(32) VF(33) VF(34)
    virtual C3DVector GetActiveCameraPosition() const { ++g_activeCalls; return g_activePos; }
    void*   ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    unsigned char _pad0a[2];
    bool    CutInto;
    bool    CutOutOf;
    bool    TestAngleBeforeActivation;
    CWorld* GetWorld() const { return &g_world; }
    bool IsAngleValid() const;
};
bool CTCCameraPointDefinitionBase::IsAngleValid() const
{
    if (TestAngleBeforeActivation) {
        CThing* hero = GetWorld()->GetCurrentPlayer()->GetControlledCreature();
        if (hero && !hero->IsDead()) {
            C3DVector camera = GetWorld()->GetCameraManager()->PeekCurrentPos();
            const C3DVector& heroPosition = hero->GetPosition();
            C3DVector toCamera;
            toCamera.X = camera.X - heroPosition.X;
            toCamera.Y = camera.Y - heroPosition.Y;
            C2DVector toActive = DeltaXY(GetActiveCameraPosition(), heroPosition);
            float d = toActive.X * toCamera.X;
            d += toActive.Y * toCamera.Y;
            return d > 0.0f;
        }
        return false;
    }
    return true;
}
static C3DVector V(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }
int main()
{
    g_world.PCameraManager = &g_cameraManager;
    CThing hero; memset(&hero, 0, sizeof(hero)); g_hero = &hero;
    CTCCameraPointDefinitionBase p; memset(&p, 0, sizeof(p)); new (&p) CTCCameraPointDefinitionBase;
    // 1. no angle test: valid, nothing queried
    p.TestAngleBeforeActivation = false; g_cameraCalls = 0; g_activeCalls = 0; g_heroPosCalls = 0;
    CHECK(p.IsAngleValid() == true); CHECK(g_cameraCalls == 0); CHECK(g_activeCalls == 0);
    p.TestAngleBeforeActivation = true;
    // 2. no hero: invalid
    g_hero = 0; CHECK(p.IsAngleValid() == false); CHECK(g_cameraCalls == 0);
    // 3. dead hero: invalid
    g_hero = &hero; hero.Flags91 = 1; CHECK(p.IsAngleValid() == false); CHECK(g_cameraCalls == 0);
    hero.Flags91 = 0;
    // 4. camera and active position on the same side of the hero (positive dot): valid
    g_heroPos = V(10, 10, 0); g_cameraPos = V(20, 10, 99); g_activePos = V(15, 12, -5);
    CHECK(p.IsAngleValid() == true); CHECK(g_cameraCalls == 1); CHECK(g_activeCalls == 1); CHECK(g_heroPosCalls == 1);
    // 5. opposite sides (negative dot): invalid
    g_activePos = V(0, 10, 0); CHECK(p.IsAngleValid() == false);
    // 6. perpendicular (dot exactly 0): invalid (strict >)
    g_activePos = V(10, 20, 0); CHECK(p.IsAngleValid() == false);
    // 7. Z is ignored: active above the hero only in Z and slightly toward the camera in Y-axis sense
    g_cameraPos = V(10, 20, 0); g_activePos = V(10, 11, 500); CHECK(p.IsAngleValid() == true);
    if (fails == 0) printf("0075c530_TEST PASS\n");
    else printf("0075c530_TEST FAIL (%d)\n", fails);
    return fails;
}