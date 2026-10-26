// Behaviour test for 00724f80: body copied from src/00724f80.cpp (CTCPhysicsControlled::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_stdCreate = 0;
class CTCPhysicsBase { public: unsigned char _pad[0x3c]; unsigned char OldMoving[4]; };
class C3DVector { public:
    float X, Y, Z;
    void Set(float x, float y, float z) { Z = z; Y = y; X = x; }
};
struct SPhysicsBaseFlags { bool Moving : 1; bool OldMoving : 1; };
struct CTCPhysicsStandard : CTCPhysicsBase {
    unsigned char _standard[0x48];
    void OnCreate() { ++g_stdCreate; }
    SPhysicsBaseFlags& MovingFlags() { return *reinterpret_cast<SPhysicsBaseFlags*>(OldMoving); }
};
struct CTCPhysicsControlled : CTCPhysicsStandard {
    C3DVector ImpulseVelocity; C3DVector ForcedMoveToPos; unsigned char ForcedFlags; float RenderZOffset; float LastRenderZOffset;
    void OnCreate();
};
void CTCPhysicsControlled::OnCreate()
{
    CTCPhysicsStandard::OnCreate();
    ImpulseVelocity.Set(0.0f, 0.0f, 0.0f);
    OldMoving[0] |= 1;
    ForcedFlags &= ~1;
}
int main()
{
    CTCPhysicsControlled tc; memset(&tc, 0x55, sizeof(tc));
    tc.ImpulseVelocity.X = 1.0f; tc.ImpulseVelocity.Y = 2.0f; tc.ImpulseVelocity.Z = 3.0f; tc.OldMoving[0] = 0x02; tc.ForcedFlags = 0x03;
    tc.OnCreate();
    CHECK(g_stdCreate == 1);
    CHECK(tc.ImpulseVelocity.X == 0.0f && tc.ImpulseVelocity.Y == 0.0f && tc.ImpulseVelocity.Z == 0.0f);
    CHECK(tc.OldMoving[0] == 0x03 && tc.ForcedFlags == 0x02);
    CHECK(tc.ForcedMoveToPos.X != 0.0f && tc.OldMoving[1] == 0x55);   // untouched neighbours
    tc.OldMoving[0] = 0x01; tc.ForcedFlags = 0x00; tc.OnCreate(); CHECK(g_stdCreate == 2 && tc.OldMoving[0] == 0x01 && tc.ForcedFlags == 0x00);
    if (fails == 0) printf("00724f80_TEST PASS\n"); else printf("00724f80_TEST FAIL\n");
    return fails;
}