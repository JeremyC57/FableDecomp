// Behaviour test for CTCAssassinRush::SetupCameraOnStick (0x007A9C80).
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& b) const { return C3DVector(X - b.X, Y - b.Y, Z - b.Z); }
};
class CCharString { public: const char* PStringData; };
CCharString g_CameraStickHelperName = { "camera_stick" };
static bool g_hasHelper = true;
static const CCharString* g_askedName = 0;
static float g_askedTime = 0.0f;
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time)
    {
        g_askedName = &name; g_askedTime = time;
        if (!g_hasHelper || index != 0 || flags != 0) return false;
        *position = C3DVector(10.0f, 20.0f, 30.0f); return true;
    }
};
static C3DVector g_heroPos(1.0f, 2.0f, 3.0f);
class CThing { public: CTCGraphicAppearance* PGraphicAppearance; const C3DVector& GetPosition() const { return g_heroPos; } };
class CGameCameraManager { public: virtual C3DVector PeekCurrentPos() const { return C3DVector(15.0f, 25.0f, 50.0f); } };
class CWorld { public: CGameCameraManager* PCameraManager; CGameCameraManager* GetCameraManager() const { return PCameraManager; } };
static CWorld* g_world = 0;
static int g_setups = 0;
class CTCAssassinRush {
public:
    CThing* ParentThing;
    C3DVector LocalRelativeVectorForCamera;
    C3DVector RelativeVectorForCamera;
    CWorld* GetWorld() { return g_world; }
    void SetupCameraFromStick_007a9b10() { ++g_setups; }
    void SetupCameraOnStick()
    {
        CTCGraphicAppearance* appearance = ParentThing->PGraphicAppearance;
        if (appearance == 0)
            return;
        C3DVector stick;
        if (!appearance->GetHelperPoint(g_CameraStickHelperName, 0, 0, &stick, 1.0f))
            return;
        C3DVector heroPosition = ParentThing->GetPosition();
        LocalRelativeVectorForCamera = stick - heroPosition;
        RelativeVectorForCamera = GetWorld()->GetCameraManager()->PeekCurrentPos() - stick;
        SetupCameraFromStick_007a9b10();
    }
};

int main()
{
    CGameCameraManager cam; CWorld world; world.PCameraManager = &cam; g_world = &world;
    CTCGraphicAppearance app; CThing thing; thing.PGraphicAppearance = &app;
    CTCAssassinRush r; r.ParentThing = &thing;
    r.LocalRelativeVectorForCamera = C3DVector(-1.0f, -1.0f, -1.0f); r.RelativeVectorForCamera = C3DVector(-1.0f, -1.0f, -1.0f);
    r.SetupCameraOnStick();
    bool ok = g_setups == 1 && g_askedName == &g_CameraStickHelperName && g_askedTime == 1.0f
        && r.LocalRelativeVectorForCamera.X == 9.0f && r.LocalRelativeVectorForCamera.Y == 18.0f && r.LocalRelativeVectorForCamera.Z == 27.0f
        && r.RelativeVectorForCamera.X == 5.0f && r.RelativeVectorForCamera.Y == 5.0f && r.RelativeVectorForCamera.Z == 20.0f;
    g_hasHelper = false; r.LocalRelativeVectorForCamera.X = 77.0f;
    r.SetupCameraOnStick();
    ok = ok && g_setups == 1 && r.LocalRelativeVectorForCamera.X == 77.0f;
    thing.PGraphicAppearance = 0; g_askedName = 0;
    r.SetupCameraOnStick();
    ok = ok && g_setups == 1 && g_askedName == 0;
    if (!ok) { std::printf("007a9c80_TEST FAIL\n"); return 1; }
    std::printf("007a9c80_TEST PASS\n");
    return 0;
}