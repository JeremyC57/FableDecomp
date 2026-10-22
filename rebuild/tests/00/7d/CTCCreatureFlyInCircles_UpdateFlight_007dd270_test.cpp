// Behaviour test for 007dd270: src/007dd270.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCCreatureFlyInCircles::UpdateFlight(C3DVector&, C3DVector&)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCreatureFlyInCircles::UpdateFlight(C3DVector& position, C3DVector& velocity) -- one step of circling flight: run the radius and speed ramps, bank by the transition progress (or a fixed quarter turn when strafing), move along the banked velocity, pull the position back onto the circle around the centre with a gentle height ease, and point the velocity along the flat step taken, banked again
// Leaf: ?UpdateFlight@CTCCreatureFlyInCircles@@IAEXAAVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DD270, 437 bytes, `ret 8`.
// Callees: CCreatureActionBase::GetProgressRatio 0x006925E0 (catalog "operator==" / landed "GetMovementRatio" are
//   propagated), C3DVector::RotateAboutZ 0x00A14040, C3DVector::Normalise 0x00A14440; constants -0.25 0x01230C8C,
//   1.0 0x0122DED8, 0.1 0x0122EE58.
// Layout: CTCCreatureFlyInCircles (Ego_r.pdb): Creature +0x0c, Centre +0x10, Speed +0x1c, DeltaSpeed +0x20,
//   SpeedChangeTicks +0x24, CurrentRadius +0x28, DeltaRadius +0x2c, RadiusChangeTicks +0x30, Mode +0x34,
//   Transitioning +0x38; CThingCreatureBase PCurrentAction +0x100 (retail).
// Shape: straight-line source with inline C3DVector operators; the bank lives in the dead position-argument slot.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    C3DVector& operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; return *this; }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    void Normalise();                                        // 0x00A14440
    void RotateAboutZ(float angle);                          // 0x00A14040
};

struct CCreatureActionBase {
    float GetProgressRatio();                                // 0x006925E0
};
struct CThingCreatureBase {
    unsigned char        _pad00[0x100];
    CCreatureActionBase* PCurrentAction;                     // +0x100 (CCountedPointer<CCreatureActionBase> data)
};

enum EFlyMode { FLY_MODE_STRAFE, FLY_MODE_LOOP, FLY_MODE_BREATHE_FIRE };

class CTCCreatureFlyInCircles {
public:
    void*               __vftable;                           // +0x00
    void*               ParentThing;                         // +0x04 (CTCBase)
    unsigned char       _pad08[0x0c - 0x08];
    CThingCreatureBase* Creature;                            // +0x0c
    C3DVector           Centre;                              // +0x10
    float               Speed;                               // +0x1c
    float               DeltaSpeed;                          // +0x20
    long                SpeedChangeTicks;                    // +0x24
    float               CurrentRadius;                       // +0x28
    float               DeltaRadius;                         // +0x2c
    long                RadiusChangeTicks;                   // +0x30
    EFlyMode            Mode;                                // +0x34
    bool                Transitioning;                       // +0x38
    void UpdateFlight(C3DVector& position, C3DVector& velocity);
};

void CTCCreatureFlyInCircles::UpdateFlight(C3DVector& position, C3DVector& velocity)
{
    C3DVector oldPosition = position;
    float bank = 0.0f;
    if (RadiusChangeTicks) {
        CurrentRadius += DeltaRadius;
        --RadiusChangeTicks;
    }
    if (SpeedChangeTicks) {
        Speed += DeltaSpeed;
        --SpeedChangeTicks;
    }
    if (Transitioning) {
        CCreatureActionBase* action = Creature->PCurrentAction;
        if (action) {
            float progress = action->GetProgressRatio();
            if (Mode == FLY_MODE_STRAFE)
                bank = progress * -0.25f;
            else
                bank = (1.0f - progress) * -0.25f;
        }
    } else if (Mode == FLY_MODE_STRAFE)
        bank = -0.25f;
    float height = position.Z;
    velocity.Z = 0.0f;
    velocity.RotateAboutZ(-bank);
    position += velocity * Speed;
    position -= Centre;
    position.Z = 0.0f;
    position.Normalise();
    position *= CurrentRadius;
    position += Centre;
    position.Z = (Centre.Z - height) * 0.1f + height;
    velocity = position - oldPosition;
    velocity.Z = 0.0f;
    velocity.Normalise();
    velocity.RotateAboutZ(bank);
}

// ---- stubs ----
static char g_log[256];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
void C3DVector::Normalise()
{
    Log("Normalise");
    float r = 1.0f / (float)sqrt(X * X + Y * Y + Z * Z);
    X *= r; Y *= r; Z *= r;
}
static float g_angles[2]; static int g_rotations = 0; static float g_rotZ = -1.0f;
void C3DVector::RotateAboutZ(float angle)
{
    char b[32]; _snprintf(b, sizeof b, "Rotate(%g)", angle); Log(b);
    if (g_rotations < 2) g_angles[g_rotations] = angle;
    ++g_rotations; g_rotZ = Z;
    double a = angle * 6.283185307179586, c = cos(a), s = sin(a);
    float x = X, y = Y;
    X = (float)(x * c - y * s); Y = (float)(x * s + y * c);
}
static float g_progress = 0.0f;
float CCreatureActionBase::GetProgressRatio() { Log("Progress"); return g_progress; }

static int Near(float a, float b) { return fabs(a - b) < 1e-4f; }
static void Reset() { g_log[0] = 0; g_step = 0; g_rotations = 0; g_angles[0] = g_angles[1] = -9.0f; }

int main()
{
    CCreatureActionBase action;
    CThingCreatureBase creature; memset(&creature, 0, sizeof creature);
    CTCCreatureFlyInCircles fly; memset(&fly, 0, sizeof fly);
    fly.Creature = &creature;
    fly.Centre = C3DVector(10, 20, 30);
    // 1: looping, no ramps: no bank; one step along +Y from (15,20,10) is pulled back onto the radius-5 circle,
    //    the height eases 10% towards the centre, velocity becomes the flat unit step
    {
        Reset(); fly.Mode = FLY_MODE_LOOP; fly.Transitioning = false; fly.Speed = 2.0f; fly.CurrentRadius = 5.0f;
        C3DVector position(15, 20, 10), velocity(0, 1, 7);
        fly.UpdateFlight(position, velocity);
        CHECK(strcmp(g_log, "1:Rotate(-0);2:Normalise;3:Normalise;4:Rotate(0);") == 0 || strcmp(g_log, "1:Rotate(0);2:Normalise;3:Normalise;4:Rotate(0);") == 0);
        float d = (float)sqrt(25.0 + 4.0);
        CHECK(Near(position.X, 10 + 5 * 5 / d) && Near(position.Y, 20 + 5 * 2 / d) && Near(position.Z, 12.0f));
        float sx = position.X - 15, sy = position.Y - 20, sl = (float)sqrt(sx * sx + sy * sy);
        CHECK(Near(velocity.X, sx / sl) && Near(velocity.Y, sy / sl) && velocity.Z == 0.0f && g_rotZ == 0.0f);
    }
    // 2: ramps step once each and count down; strafing out of a transition banks a quarter turn
    {
        Reset(); fly.Mode = FLY_MODE_STRAFE; fly.Transitioning = false;
        fly.Speed = 1.0f; fly.DeltaSpeed = 0.5f; fly.SpeedChangeTicks = 2;
        fly.CurrentRadius = 4.0f; fly.DeltaRadius = 1.0f; fly.RadiusChangeTicks = 1;
        C3DVector position(15, 20, 30), velocity(0, 1, 0);
        fly.UpdateFlight(position, velocity);
        CHECK(fly.Speed == 1.5f && fly.SpeedChangeTicks == 1 && fly.CurrentRadius == 5.0f && fly.RadiusChangeTicks == 0);
        CHECK(g_angles[0] == 0.25f && g_angles[1] == -0.25f);
        // the first rotation turns +Y by +0.25 turn to -X, so the step is (15 - 1.5, 20) and stays on the radius-5 circle
        CHECK(Near(position.X, 15.0f) && Near(position.Y, 20.0f) && Near(position.Z, 30.0f));
    }
    // 3: transitioning with an action: strafe banks by progress * -0.25, the other modes by (1 - progress) * -0.25
    {
        Reset(); creature.PCurrentAction = &action; g_progress = 0.4f; fly.Transitioning = true; fly.Mode = FLY_MODE_STRAFE;
        fly.SpeedChangeTicks = 0; fly.RadiusChangeTicks = 0;
        C3DVector position(15, 20, 30), velocity(0, 1, 0);
        fly.UpdateFlight(position, velocity);
        CHECK(strncmp(g_log, "1:Progress;", 11) == 0 && Near(g_angles[0], 0.1f) && Near(g_angles[1], -0.1f));
        Reset(); fly.Mode = FLY_MODE_BREATHE_FIRE;
        fly.UpdateFlight(position, velocity);
        CHECK(Near(g_angles[0], 0.15f) && Near(g_angles[1], -0.15f));
    }
    // 4: transitioning without an action: no bank, even when strafing
    {
        Reset(); creature.PCurrentAction = 0; fly.Mode = FLY_MODE_STRAFE;
        C3DVector position(15, 20, 30), velocity(0, 1, 0);
        fly.UpdateFlight(position, velocity);
        CHECK(g_angles[0] == 0.0f && g_angles[1] == 0.0f && strstr(g_log, "Progress") == 0);
    }
    if (fails == 0) printf("007dd270_TEST PASS\n");
    return fails;
}