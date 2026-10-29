// Behaviour test for 0x0093B1D0 CAIStateGroup_BanditSitRoundCampfire::Update (model copy of the control flow;
// engine services, actions and RNG are stubs that log what the state group asks for).
#include <cstdio>

class C2DVector;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator+(const C2DVector& v) const;
    C3DVector& operator+=(const C2DVector& v);
};
class C2DVector { public:
    float X, Y;
    C2DVector() : X(0), Y(0) { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
    C2DVector& Normalise()
    {
        float l = X * X + Y * Y;
        if (l > 0) {   // test data uses axis-aligned vectors, so no sqrt is needed
            float s = (X != 0 ? (X > 0 ? X : -X) : (Y > 0 ? Y : -Y));
            X /= s; Y /= s;
        }
        return *this;
    }
    void Rotate(float) { }   // stub: identity rotation
};
C3DVector C3DVector::operator+(const C2DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z); }
C3DVector& C3DVector::operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
class CRightHandedSet { public: C3DVector Up, Forward; };

// --- stubs ----------------------------------------------------------------------------------------------------
static char g_log[32]; static int g_logn = 0;
static void Log(char c) { g_log[g_logn++] = c; g_log[g_logn] = 0; }
static bool g_freePoint = true; static int g_navOkAt = -1; static int g_navTries = 0;
static int g_move = 2, g_look = 2, g_sheathe = 2, g_sit = 2; static bool g_weapon = false;
static C3DVector g_moveTarget, g_lookTarget; static float g_rnd = 2.0f;
static unsigned long g_frame = 100; static long g_fps = 30;
static int g_modeAlive = 0;

class CCPPointerInfo { public: unsigned long RefCount; };
struct CMode { CMode() { ++g_modeAlive; } ~CMode() { --g_modeAlive; } };
struct CCountedPointer {
    CMode* Data; CCPPointerInfo* Info;
    CCountedPointer(CMode* d) : Data(d), Info(0) { if (d) { Info = new CCPPointerInfo; Info->RefCount = 1; } }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info && --Info->RefCount == 0) { delete Data; delete Info; } Data = 0; Info = 0; }
};
static unsigned long g_sitRefs = 0;

struct CAIStateGroup_BanditSitRoundCampfire {
    long SeatPosIndex; bool GotSeatPos; bool Sitting; unsigned long FrameToNextCheckIsRunnable;
    bool WasCarryingWeapon; C3DVector CampfirePos; C3DVector CreaturePos;
    CAIStateGroup_BanditSitRoundCampfire() : SeatPosIndex(-1), GotSeatPos(false), Sitting(false),
        FrameToNextCheckIsRunnable(0), WasCarryingWeapon(false) { }
    void Terminate() { Log('T'); }
    bool SitMain() { Log('M'); return false; }
    bool GetFreeActionPoint(C3DVector& pos, CRightHandedSet& o, long& index)
    {
        if (!g_freePoint) return false;
        pos = C3DVector(10, 0, 5); o.Forward = C3DVector(0, 1, 0); index = 3; return true;
    }
    bool IsPosNavigable(const C3DVector&) { return g_navTries++ == g_navOkAt; }
    int MoveToPos(const C3DVector& p, bool& arrived) { Log('V'); g_moveTarget = p; arrived = true; return g_move; }
    int LookAtPos(const C3DVector& p) { Log('L'); g_lookTarget = p; return g_look; }
    int Sheathe() { Log('S'); return g_sheathe; }
    int SitDown(CCountedPointer mode) { Log('D'); g_sitRefs = mode.Info->RefCount; return g_sit; }
    void Update();
};

void CAIStateGroup_BanditSitRoundCampfire::Update()
{
    FrameToNextCheckIsRunnable = g_frame + 5 * g_fps;
    C3DVector pos;
    CRightHandedSet orientation;
    bool arrived;
    if (!GetFreeActionPoint(pos, orientation, SeatPosIndex)) {
        Terminate();
        return;
    }
    C2DVector dir = C2DVector(pos - CampfirePos).Normalise();
    for (long i = 0; i < 5; ++i) {
        C2DVector offset = dir * g_rnd;
        offset.Rotate(g_rnd);
        if (IsPosNavigable(pos + offset)) {
            pos += offset;
            break;
        }
    }
    GotSeatPos = true;
    if (MoveToPos(pos, arrived) != 2) {
        Terminate();
        return;
    }
    C3DVector forward = orientation.Forward;
    C3DVector lookAt = forward + CreaturePos;
    if (LookAtPos(lookAt) != 2) {
        Terminate();
        return;
    }
    if ((WasCarryingWeapon = g_weapon) != false && Sheathe() != 2) {
        Terminate();
        return;
    }
    CCountedPointer sitMode(new CMode);
    Sitting = true;
    if (SitDown(sitMode) != 2) {
        Terminate();
        return;
    }
    SitMain();
    Terminate();
}

static void Reset() { g_logn = 0; g_log[0] = 0; g_navTries = 0; }
static bool Eq(const char* a) { int i = 0; for (; a[i]; ++i) if (g_log[i] != a[i]) return false; return g_log[i] == 0; }

int main()
{
    bool ok = true;
    { // no free seat: terminate at once
        CAIStateGroup_BanditSitRoundCampfire g; Reset(); g_freePoint = false;
        g.Update();
        ok = ok && Eq("T") && !g.GotSeatPos && g.FrameToNextCheckIsRunnable == 250;
        g_freePoint = true;
    }
    { // third try navigable, walk fails
        CAIStateGroup_BanditSitRoundCampfire g; Reset(); g_navOkAt = 2; g_move = 1;
        g.Update();
        ok = ok && Eq("VT") && g.GotSeatPos && g.SeatPosIndex == 3 && g_moveTarget.X == 12.0f && g_moveTarget.Z == 5.0f;
        g_move = 2;
    }
    { // no navigable try: seat unchanged; full sit with weapon sheathed
        CAIStateGroup_BanditSitRoundCampfire g; Reset(); g_navOkAt = -1; g_weapon = true;
        g.CreaturePos = C3DVector(1, 2, 3);
        g.Update();
        ok = ok && Eq("VLSDMT") && g_navTries == 5 && g_moveTarget.X == 10.0f && g_lookTarget.Y == 3.0f
            && g.Sitting && g.WasCarryingWeapon && g_sitRefs == 2 && g_modeAlive == 0;
    }
    { // sheathe fails: no sit
        CAIStateGroup_BanditSitRoundCampfire g; Reset(); g_sheathe = 1;
        g.Update();
        ok = ok && Eq("VLST") && !g.Sitting;
        g_sheathe = 2; g_weapon = false;
    }
    { // sit down refused: terminate without SitMain
        CAIStateGroup_BanditSitRoundCampfire g; Reset(); g_sit = 1;
        g.Update();
        ok = ok && Eq("VLDT") && g.Sitting && !g.WasCarryingWeapon && g_modeAlive == 0;
        g_sit = 2;
    }
    std::printf(ok ? "0093b1d0_TEST PASS\n" : "0093b1d0_TEST FAIL\n");
    return ok ? 0 : 1;
}