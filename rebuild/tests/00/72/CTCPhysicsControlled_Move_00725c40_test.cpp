// Behaviour test for CTCPhysicsControlled::Move (0x00725c40): teleport branch, physics branch call order, margin choice, impulse cleared.
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#pragma intrinsic(sqrt)
#include <cstdlib>
#include <cstdio>
#include <cstring>
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
};
struct C2DBoxI { long Left; long Top; long Right; long Bottom; };
class CThing;
struct CThingPtrVector {                            // retail STLport vector<CThing*> (12 bytes)
    CThing** first;
    CThing** last;
    CThing** endOfStorage;
    CThingPtrVector() : first(0), last(0), endOfStorage(0) { }
    ~CThingPtrVector() { if (first) std::free(first); }
};
struct CPhysicsDef;
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                // +0x20
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};
#define V4(n) virtual void v##n##a() { } virtual void v##n##b() { } virtual void v##n##c() { } virtual void v##n##d() { }
struct CTCPhysicsControlled {
    V4(0) V4(1) V4(2) V4(3) V4(4) V4(5) V4(6)        // slots 0-27
    virtual void v28() { } virtual void v29() { }
    virtual void SetPosition(const C3DVector& pos); // slot 30 (+0x78)
    virtual void v31() { }
    V4(8) V4(9) V4(10) V4(11) V4(12) V4(13) V4(14) V4(15) V4(16) V4(17) V4(18) V4(19) V4(20) V4(21) V4(22) V4(23)   // 32-95
    virtual void v96() { }
    virtual void UpdateOnSurface(CThingPtrVector& things, C3DVector& position);   // slot 97 (+0x184)
    virtual C2DBoxI GetPossibleCollidedWithArea();  // slot 98 (+0x188)
    CThing* Owner;                                  // +0x04
    long _pad08;
    C3DVector Position;                             // +0x0c
    C3DVector OldPosition;                          // +0x18
    long OldPositionLastFrameSet;                   // +0x24
    C3DVector Velocity;                             // +0x28
    float Radius;                                   // +0x34
    CPhysicsDef* PhysicsDef;                        // +0x38
    unsigned char Moving : 1;                       // +0x3c bit 0
    unsigned char _pad3d[0x88 - 0x3d];
    C3DVector ImpulseVelocity;                      // +0x88
    C3DVector ForcedMoveToPos;                      // +0x94
    unsigned char ForcedMoveToPosSet : 1;           // +0xa0 bit 0
    unsigned char ForcedMoveToPosUsePhysics : 1;    // +0xa0 bit 1
    void PerformForceUpdates();                     // 0x008E5670
    void GetPossibleInteractedWithList(const C2DBoxI& area, CThingPtrVector& things, float speed);   // 0x008E57D0
    void UpdateVelocityForImpassability(const C3DVector& position, C3DVector& velocity);       // 0x007243C0
    void PerformStepBasedMovement(CThingPtrVector& things, C3DVector& position);              // 0x008E5880
    void UpdateIsMoving();                          // 0x008E5270
    void AdjustThingListForDebugHelpers(CThingPtrVector& things);                            // 0x007254B0
    void PerformImpulseBasedMovement(CThingPtrVector& things, C3DVector& position);           // 0x00725500
    void UpdateAdditionalImpulse();                 // 0x00725B70
    void Move();
};

void CTCPhysicsControlled::Move()
{
    PerformForceUpdates();
    C3DVector position(Position);
    if (ForcedMoveToPosSet) {
        ForcedMoveToPosSet = false;
        if (!ForcedMoveToPosUsePhysics) {
            position = ForcedMoveToPos;
            ImpulseVelocity.Z = 0.0f;
            ImpulseVelocity.Y = 0.0f;
            ImpulseVelocity.X = 0.0f;
            goto commit;
        }
        ImpulseVelocity += ForcedMoveToPos - Position;
    }
    {
        C2DBoxI area = GetPossibleCollidedWithArea();
        float impulseSpeedSquared = ImpulseVelocity.GetLengthSquared();
        CThingPtrVector things;
        float speed = Velocity.GetLength();
        float margin = 0.5f;
        const CThing* owner = Owner;
        if (owner->HasTC(0xa8) || owner->HasTC(0xa9))
            margin = 4.5f;
        GetPossibleInteractedWithList(area, things, (float)sqrt(impulseSpeedSquared) + margin + speed);
        AdjustThingListForDebugHelpers(things);
        PerformImpulseBasedMovement(things, position);
        ImpulseVelocity.Z = 0.0f;
        ImpulseVelocity.Y = 0.0f;
        ImpulseVelocity.X = 0.0f;
        UpdateVelocityForImpassability(position, Velocity);
        PerformStepBasedMovement(things, position);
        UpdateOnSurface(things, position);
        UpdateIsMoving();
    }
commit:
    SetPosition(position);
    UpdateAdditionalImpulse();
}

static char g_log[256];
static int g_n = 0;
static float g_lastSpeed = 0.0f;
static C3DVector g_committed;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
void CTCPhysicsControlled::PerformForceUpdates() { Log('F'); }
void CTCPhysicsControlled::GetPossibleInteractedWithList(const C2DBoxI& area, CThingPtrVector& things, float speed)
{
    Log('G'); g_lastSpeed = speed;
    if (area.Left != 7 || things.first != 0) Log('!');
    things.first = (CThing**)std::malloc(8); things.last = things.first; things.endOfStorage = things.first + 2;
}
void CTCPhysicsControlled::UpdateVelocityForImpassability(const C3DVector& position, C3DVector& velocity)
{
    Log('V');
    if (ImpulseVelocity.X != 0.0f || ImpulseVelocity.Y != 0.0f || ImpulseVelocity.Z != 0.0f) Log('!');
    if (&velocity != &Velocity) Log('!');
    (void)position;
}
void CTCPhysicsControlled::PerformStepBasedMovement(CThingPtrVector& things, C3DVector& position) { Log('S'); position.X += 1.0f; (void)things; }
void CTCPhysicsControlled::UpdateIsMoving() { Log('M'); }
void CTCPhysicsControlled::AdjustThingListForDebugHelpers(CThingPtrVector& things) { Log('A'); if (!things.first) Log('!'); }
void CTCPhysicsControlled::PerformImpulseBasedMovement(CThingPtrVector& things, C3DVector& position) { Log('I'); position.Y += ImpulseVelocity.Y; (void)things; }
void CTCPhysicsControlled::UpdateAdditionalImpulse() { Log('U'); }
void CTCPhysicsControlled::SetPosition(const C3DVector& pos) { Log('P'); g_committed = pos; }
void CTCPhysicsControlled::UpdateOnSurface(CThingPtrVector& things, C3DVector& position) { Log('O'); (void)things; (void)position; }
C2DBoxI CTCPhysicsControlled::GetPossibleCollidedWithArea() { Log('C'); C2DBoxI b; b.Left = 7; b.Top = 0; b.Right = 0; b.Bottom = 0; return b; }
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { std::printf("FAIL %s (%s)\n", what, g_log); ++fails; } }
static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    CThing owner; for (int i = 0; i < 9; ++i) owner.m_presentBits[i] = 0;
    CTCPhysicsControlled p;
    p.Owner = &owner;
    p.Position = C3DVector(1.0f, 2.0f, 3.0f);
    p.Velocity = C3DVector(3.0f, 4.0f, 0.0f);
    p.ImpulseVelocity = C3DVector(0.0f, 0.0f, 0.0f);
    p.ForcedMoveToPos = C3DVector(10.0f, 20.0f, 30.0f);
    p.ForcedMoveToPosSet = 1; p.ForcedMoveToPosUsePhysics = 0;
    p.Move();                                            // teleport: no physics, impulse cleared
    check(std::strcmp(g_log, "FPU") == 0, "teleport order");
    check(g_committed.X == 10.0f && g_committed.Y == 20.0f && g_committed.Z == 30.0f && !p.ForcedMoveToPosSet, "teleport target");
    g_n = 0; g_log[0] = 0;
    p.Position = C3DVector(0.0f, 0.0f, 0.0f);
    p.ImpulseVelocity = C3DVector(0.0f, 0.0f, 0.0f);
    p.ForcedMoveToPos = C3DVector(0.0f, 6.0f, 8.0f);    // impulse += (0,6,8): length 10
    p.ForcedMoveToPosSet = 1; p.ForcedMoveToPosUsePhysics = 1;
    p.Move();
    check(std::strcmp(g_log, "FCGAIVSOMPU") == 0, "physics order");
    check(near(g_lastSpeed, 10.0f + 0.5f + 5.0f), "speed margin 0.5");
    check(near(g_committed.Y, 6.0f) && near(g_committed.X, 1.0f), "impulse then step applied");
    check(p.ImpulseVelocity.X == 0.0f && p.ImpulseVelocity.Y == 0.0f && p.ImpulseVelocity.Z == 0.0f, "impulse cleared");
    g_n = 0; g_log[0] = 0;
    owner.m_presentBits[5] = 1UL << 9;                  // interface 0xA9
    p.Move();
    check(std::strcmp(g_log, "FCGAIVSOMPU") == 0, "plain order");
    check(near(g_lastSpeed, 0.0f + 4.5f + 5.0f), "speed margin 4.5");
    if (fails == 0) std::printf("00725c40_TEST PASS\n");
    return fails;
}