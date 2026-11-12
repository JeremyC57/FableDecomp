// Behaviour test for CCombatSequence_WillForcePushLongRange::IsValid (0x00909A00): false only when the creature has a will-power
// indicator that is not powered up; the camera-facing computation runs for a powered-up one but both outcomes are valid.
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void Normalise() { float f = 1.0f / (float)std::sqrt(X * X + Z * Z + Y * Y); X *= f; Y *= f; Z *= f; }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
};
class CCamera { public: C3DVector WorldPosition; C3DVector Up; C3DVector Forward; const C3DVector& PeekViewDirection() const { return Forward; } };
static int g_posCalls = 0, g_cameraCalls = 0;
static float g_lastDot = 0.0f;
class CAIGameCameraBase { public:
    C3DVector Pos; CCamera Camera;
    C3DVector PeekCurrentPos() const { ++g_posCalls; return Pos; }
    CCamera GetRenderCamera(float interpolation) const { if (interpolation == 1.0f) ++g_cameraCalls; return Camera; }
};
static CAIGameCameraBase g_camera;
class CTCAICreatureWillPowerIndicator { public: bool PoweredUp; bool IsPoweredUp() const { return PoweredUp; } };
class CThingCreatureBase { public:
    C3DVector Position; CTCAICreatureWillPowerIndicator* Indicator;
    bool PeekTC(const CTCAICreatureWillPowerIndicator** out) const { if (!Indicator) return false; *out = Indicator; return true; }
    const C3DVector& GetPos() const { return Position; }
};
struct CCombatSequence_WillForcePushLongRange {
    CThingCreatureBase* Creature;
    bool IsValid() const
    {
        const CThingCreatureBase& creature = *Creature;
        const CTCAICreatureWillPowerIndicator* indicator;
        if (creature.PeekTC(&indicator)) {
            if (indicator->IsPoweredUp()) {
                C3DVector to_creature = creature.GetPos() - g_camera.PeekCurrentPos();
                to_creature.Z = 0.0f;
                to_creature.Normalise();
                C3DVector view = g_camera.GetRenderCamera(1.0f).PeekViewDirection();
                view.Z = 0.0f;
                g_lastDot = view.Dot(to_creature);
                if (g_lastDot < 0.0f)
                    return true;
            } else
                return false;
        }
        return true;
    }
};
int main()
{
    g_camera.Pos = C3DVector(0.0f, 0.0f, 5.0f);
    g_camera.Camera.Forward = C3DVector(0.0f, 1.0f, -0.5f);
    CTCAICreatureWillPowerIndicator indicator = { false };
    CThingCreatureBase creature; creature.Position = C3DVector(3.0f, 4.0f, 9.0f); creature.Indicator = 0;
    CCombatSequence_WillForcePushLongRange sequence = { &creature };
    bool ok = true;
    ok = ok && sequence.IsValid() && g_posCalls == 0 && g_cameraCalls == 0;            // no indicator: valid, no camera work
    creature.Indicator = &indicator;
    ok = ok && !sequence.IsValid() && g_posCalls == 0 && g_cameraCalls == 0;           // not powered up: invalid
    indicator.PoweredUp = true;
    ok = ok && sequence.IsValid() && g_posCalls == 1 && g_cameraCalls == 1;            // in front of the camera
    ok = ok && std::fabs(g_lastDot - 0.8f) < 1e-4f;                                    // flat (3,4) normalised . (0,1)
    creature.Position = C3DVector(3.0f, -4.0f, 1.0f);
    ok = ok && sequence.IsValid() && g_posCalls == 2 && g_cameraCalls == 2;            // behind the camera: still valid
    ok = ok && std::fabs(g_lastDot + 0.8f) < 1e-4f;
    if (!ok) { std::printf("00909a00_TEST FAIL\n"); return 1; }
    std::printf("00909a00_TEST PASS\n");
    return 0;
}