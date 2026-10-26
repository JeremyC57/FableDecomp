// Behaviour test for 0066fc90 CTCCoopSpirit::UpdateScreenRestrictingForces: final.cpp body verbatim; the display, camera,
// player chain, vector maths and creature action are stubs recording their calls.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
//   and the force-ctor load order).
// pixels of an edge push the owning hero's creature back towards the middle (rotated into its movement frame, scaled and
// clamped); count the frames the spirit spends off screen and after five seconds' worth make it return to the hero
// Leaf: UpdateScreenRestrictingForces
//
// Retail 0x0066FC90, 655 bytes, plain `ret`.
// Callees: the display's GetScreenArea @ 0x00434B50 (C2DBoxF by hidden pointer), CThing::GetPosition @ 0x004C73D0, the
// camera's slot 13 (+0x34, world to screen), CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetPlayer @ 0x004498C0,
// CPlayer::GetHeroCreature @ 0x00487DC0, CThingPlayerCreature::GetCurrentRelativeMovementAngleXY @ 0x006A9640,
// C3DVector::RotateAboutZ @ 0x00A14040 / GetLength @ 0x00A14480, CTCPhysicsBase::ApplyAcceleration @ 0x006B0210, __ftol2
// @ 0x00BFEA70, CCreatureAction_SpiritReturnToHero @ 0x00854660, CThingCreatureBase::SetCurrentAction @ 0x006644F0, the
// action's base dtor @ 0x00693EF0.
// Constants: 1.1f, 25.0f, 0.04f, -0.04f, 0.8f, 1e-4f, 3.0f; globals: the main game component @ 0x013B86A0, the frame rate
// @ 0x01375550, the spirit's off-screen seconds (5.0f) @ 0x0137EA24.
// Layout (local model): ParentThing +0x04 (player number byte +0x90), OffScreenFrames +0x14; the game component's display
// +0x28, the display's camera +0x2c.
#include <math.h>
#pragma intrinsic(fabs)
class C2DVector { public: float X, Y; };
class C2DBoxF { public: C2DVector TopLeft, BottomRight; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void RotateAboutZ(float angle);                          // 0x00A14040
    float GetLength() const;                                 // 0x00A14480
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
#define VF(n) virtual void v##n();
class CCamera { public:
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12)
    virtual bool WorldToScreen(const C3DVector& world, C2DVector* screen);   // slot 13 (+0x34)
};
#undef VF
struct CDisplay {
    unsigned char _pad00[0x2c];
    CCamera* PCamera;                                        // +0x2c
    C2DBoxF GetScreenArea() const;                           // 0x00434B50
};
class CMainGameComponent { public:
    unsigned char _pad00[0x28];
    CDisplay* PDisplay;                                      // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;             // 0x013B86A0
extern long g_framesPerSecond;                               // 0x01375550
extern float g_spiritOffScreenSeconds;                       // 0x0137EA24
class CTCPhysicsBase { public: void ApplyAcceleration(const C3DVector& acceleration); };   // 0x006B0210
class CThing { public:
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    unsigned char   _pad64[0x90 - 0x64];
    char            PlayerNumber;                            // +0x90
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    float GetCurrentRelativeMovementAngleXY() const;         // 0x006A9640 (CThingPlayerCreature)
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                                  // 0x00693EF0
};
class CThingCreatureBase;
class CCreatureAction_SpiritReturnToHero : public CCreatureActionBase { public:
    CCreatureAction_SpiritReturnToHero(CThingCreatureBase* owner);   // 0x00854660
};
class CThingCreatureBase : public CThing { public:
    void SetCurrentAction(const CCreatureActionBase& action);        // 0x006644F0
};
class CPlayer { public: CThing* GetHeroCreature(); };               // 0x00487DC0
class CPlayerManager { public: CPlayer* GetPlayer(long number); };  // 0x004498C0
class CTCCoopSpirit { public:
    virtual void v00();
    CThingCreatureBase* ParentThing;                         // +0x04
    unsigned char _pad08[0x14 - 0x08];
    long          OffScreenFrames;                           // +0x14
    CPlayerManager* GetPlayerManager();                      // 0x00686D40
    void UpdateScreenRestrictingForces();
};

void CTCCoopSpirit::UpdateScreenRestrictingForces()
{
    CDisplay* display = g_pMainGameComponent->PDisplay;
    CCamera* camera = display->PCamera;
    C2DBoxF area = display->GetScreenArea();
    const C3DVector& position = ParentThing->GetPosition();
    C3DVector spiritPos;
    spiritPos.X = position.X;
    spiritPos.Y = position.Y;
    spiritPos.Z = position.Z + 1.1f;
    C2DVector screenPos;
    camera->WorldToScreen(spiritPos, &screenPos);
    float forceX = 0.0f;
    float left = area.TopLeft.X + 25.0f;
    if (screenPos.X < left) {
        forceX = (left - screenPos.X) * 0.04f;
    } else {
        float right = area.BottomRight.X - 25.0f;
        if (screenPos.X > right)
            forceX = (right - screenPos.X) * 0.04f;
    }
    float forceY = 0.0f;
    float top = area.TopLeft.Y + 25.0f;
    if (screenPos.Y < top) {
        forceY = (top - screenPos.Y) * -0.04f;
    } else {
        float bottom = area.BottomRight.Y - 25.0f;
        if (screenPos.Y > bottom)
            forceY = (bottom - screenPos.Y) * -0.04f;
    }
    if ((float)fabs(forceX) > 0.0001f || (float)fabs(forceY) > 0.0001f) {
        long player = ParentThing->PlayerNumber;
        CThing* hero = GetPlayerManager()->GetPlayer(player)->GetHeroCreature();
        float angle = hero->GetCurrentRelativeMovementAngleXY();
        C3DVector force(forceX, forceY, 0.0f);
        force.RotateAboutZ(angle);
        force *= 0.8f;
        float length = force.GetLength();
        if (length > 0.0001f) {
            if (length > 3.0f)
                length = 3.0f;
            force *= length;
            hero->PhysicsTC->ApplyAcceleration(force);
        }
    }
    if (screenPos.X < area.TopLeft.X || screenPos.X > area.BottomRight.X || screenPos.Y < area.TopLeft.Y
        || screenPos.Y > area.BottomRight.Y)
        ++OffScreenFrames;
    else
        OffScreenFrames = 0;
    if (OffScreenFrames > (long)((float)g_framesPerSecond * g_spiritOffScreenSeconds)) {
        OffScreenFrames = 0;
        CThingCreatureBase* parent = ParentThing;
        parent->SetCurrentAction(CCreatureAction_SpiritReturnToHero(parent));
    }
}

#define VS(n) void CCamera::v##n() {}
VS(0) VS(1) VS(2) VS(3) VS(4) VS(5) VS(6) VS(7) VS(8) VS(9) VS(10) VS(11) VS(12)
static C2DVector g_screen;
static C3DVector g_world;
bool CCamera::WorldToScreen(const C3DVector& world, C2DVector* screen) { g_world = world; *screen = g_screen; return true; }
C2DBoxF CDisplay::GetScreenArea() const { C2DBoxF b; b.TopLeft.X = 0; b.TopLeft.Y = 0; b.BottomRight.X = 640; b.BottomRight.Y = 480; return b; }
static C3DVector g_pos(1.0f, 2.0f, 3.0f);
const C3DVector& CThing::GetPosition() const { return g_pos; }
float CThing::GetCurrentRelativeMovementAngleXY() const { return 0.5f; }
static float g_rotAngle = -1.0f;
void C3DVector::RotateAboutZ(float angle) { g_rotAngle = angle; }
float C3DVector::GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
static int g_accel = 0;
static C3DVector g_accelVec;
void CTCPhysicsBase::ApplyAcceleration(const C3DVector& a) { ++g_accel; g_accelVec = a; }
static CTCPhysicsBase g_physics;
static CThing g_hero;
static long g_playerAsked = -99;
CThing* CPlayer::GetHeroCreature() { return &g_hero; }
static CPlayer g_player;
CPlayer* CPlayerManager::GetPlayer(long number) { g_playerAsked = number; return &g_player; }
static CPlayerManager g_pm;
CPlayerManager* CTCCoopSpirit::GetPlayerManager() { return &g_pm; }
void CTCCoopSpirit::v00() {}
void CCreatureActionBase::v00() {}
CCreatureActionBase::~CCreatureActionBase() {}
static CThingCreatureBase* g_owner = 0;
CCreatureAction_SpiritReturnToHero::CCreatureAction_SpiritReturnToHero(CThingCreatureBase* owner) { g_owner = owner; }
static int g_setAction = 0;
void CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) { ++g_setAction; }
static CDisplay g_display;
static CMainGameComponent g_mgc;
static CCamera g_camera;
CMainGameComponent* g_pMainGameComponent = &g_mgc;
long g_framesPerSecond = 10;
float g_spiritOffScreenSeconds = 5.0f;

static int Check()
{
    g_mgc.PDisplay = &g_display;
    g_display.PCamera = &g_camera;
    g_hero.PhysicsTC = &g_physics;
    CThingCreatureBase parent;
    parent.PlayerNumber = -1;
    CTCCoopSpirit spirit;
    spirit.ParentThing = &parent;
    spirit.OffScreenFrames = 0;
    // centre of the screen: no force, on screen
    g_screen.X = 320; g_screen.Y = 240;
    spirit.UpdateScreenRestrictingForces();
    if (g_accel != 0 || spirit.OffScreenFrames != 0) return 1;
    if (g_world.X != 1.0f || g_world.Y != 2.0f || g_world.Z != 3.0f + 1.1f) return 2;
    // near the left edge: push right, X force = (25 - 5) * 0.04 = 0.8, * 0.8 = 0.64, length 0.64 -> * 0.64
    g_screen.X = 5; g_screen.Y = 240;
    spirit.UpdateScreenRestrictingForces();
    if (g_accel != 1 || g_playerAsked != -1 || g_rotAngle != 0.5f) return 3;
    {
        float fx = (25.0f - 5.0f) * 0.04f * 0.8f;
        float expect = fx * fx;
        float d = g_accelVec.X - expect; if (d < 0) d = -d;
        if (d > 1e-5f || g_accelVec.Y != 0.0f || g_accelVec.Z != 0.0f) return 4;
    }
    // off screen above: Y force negative scale, and counting frames
    g_screen.X = 320; g_screen.Y = -100;
    for (int i = 0; i < 50; ++i) spirit.UpdateScreenRestrictingForces();
    if (spirit.OffScreenFrames != 50 || g_setAction != 0) return 5;
    if (!(g_accelVec.Y > 3.0f * 0.0f) && !(g_accelVec.Y < 0.0f)) return 6;
    spirit.UpdateScreenRestrictingForces();
    if (spirit.OffScreenFrames != 0 || g_setAction != 1 || g_owner != &parent) return 7;
    // clamp: huge push gives length clamped to 3
    g_screen.X = -10000; g_screen.Y = 240;
    spirit.UpdateScreenRestrictingForces();
    {
        float fx = (25.0f + 10000.0f) * 0.04f * 0.8f;
        float d = g_accelVec.X - fx * 3.0f; if (d < 0) d = -d;
        if (d > 1e-2f) return 8;
    }
    return 0;
}

int main()
{
    int r = Check();
    if (r == 0) printf("0066fc90_TEST PASS\n"); else printf("0066fc90_TEST FAIL %d\n", r);
    return r;
}