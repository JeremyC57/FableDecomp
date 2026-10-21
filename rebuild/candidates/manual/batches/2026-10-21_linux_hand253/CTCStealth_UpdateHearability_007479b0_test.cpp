#include <cstdio>
#include <cstring>
// Behaviour test for CTCStealth::UpdateHearability (0x007479B0): the body of src/ over stand-ins -- silent when sneaking or
// still, the pace multiplier picked against run/walk speeds, the modifier applied and a SIMPLE_NOISE handed the owner.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
static const float g_epsilon_0129ba3c = 1e-4f;
struct CCharString {
    char text[16];
    CCharString(const char* t, long length = -1) { strcpy(text, t); }
};
static char g_lookedUp[16];
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) { strcpy(g_lookedUp, name.text); return 77; }
};
static CDefinitionManager g_defs;
struct CTCPhysicsBase { C3DVector Position; C3DVector OldPosition; };
class CThing {
public:
    float walk, run;
    CTCPhysicsBase* PhysicsTC;
    unsigned char m_flags91;
    CThing* passedTo;
    C3DVector pos;
    float GetWalkSpeed(const C3DVector& p) { return walk; }
    float GetRunSpeed(const C3DVector& p) { return run; }
    const C3DVector& GetPosition() const { return pos; }
};
static long g_noiseDef; static float g_noiseRadius; static C3DVector g_noisePos; static int g_noises;
struct CThingNoise : CThing {
    static CThingNoise* Create(long defIndex, const C3DVector& p, float radius);
    void PassToNoiseComponent(CThing* thing) { passedTo = thing; }
};
static CThingNoise g_noise;
CThingNoise* CThingNoise::Create(long defIndex, const C3DVector& p, float radius)
{
    g_noiseDef = defIndex; g_noisePos = p; g_noiseRadius = radius; ++g_noises; return &g_noise;
}
struct CStealthDef { float BaseHearingDistance, RunningMultiplier, WalkingMultiplier, CreepingMultiplier; };
struct CTCStealth {
    CThing* ParentThing;
    bool Sneaking;
    float DistanceCanBeHeardFrom;
    CStealthDef* PDef;
    CDefinitionManager* GetDefinitions() const { return &g_defs; }
    float GetHearabilityModifier() { return 0.5f; }
    void UpdateHearability();
};

void CTCStealth::UpdateHearability()
{
    DistanceCanBeHeardFrom = 0.0f;
    if (Sneaking || !PDef)
        return;
    CThing* owner = ParentThing;
    CTCPhysicsBase* physics = owner->PhysicsTC;
    C3DVector movement(physics->Position.X - physics->OldPosition.X, physics->Position.Y - physics->OldPosition.Y,
                       physics->Position.Z - physics->OldPosition.Z);
    float speedSquared = movement.Z * movement.Z;
    speedSquared += movement.Y * movement.Y;
    speedSquared += movement.X * movement.X;
    if (speedSquared > g_epsilon_0129ba3c) {
        DistanceCanBeHeardFrom = PDef->BaseHearingDistance;
        float multiplier;
        float runSpeed = owner->GetRunSpeed(owner->GetPosition()) + g_epsilon_0129ba3c;
        if (speedSquared > runSpeed * runSpeed)
            multiplier = PDef->RunningMultiplier;
        else {
            float walkSpeed = owner->GetWalkSpeed(owner->GetPosition()) + g_epsilon_0129ba3c;
            if (speedSquared > walkSpeed * walkSpeed)
                multiplier = PDef->WalkingMultiplier;
            else
                multiplier = PDef->CreepingMultiplier;
        }
        DistanceCanBeHeardFrom *= multiplier;
        DistanceCanBeHeardFrom *= GetHearabilityModifier();
        long defIndex;
        {
            CCharString noiseName("SIMPLE_NOISE");
            defIndex = GetDefinitions()->GetDefGlobalIndexFromName(noiseName);
        }
        CThingNoise* noise = CThingNoise::Create(defIndex, ParentThing->GetPosition(), DistanceCanBeHeardFrom);
        if (noise && !(noise->m_flags91 & 1))
            noise->PassToNoiseComponent(ParentThing);
    }
}

static int fail(int line) { printf("007479b0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CTCPhysicsBase phys;
    phys.OldPosition = C3DVector(0, 0, 0);
    phys.Position = C3DVector(0, 0, 0);
    CThing owner;
    owner.walk = 1.0f; owner.run = 3.0f; owner.PhysicsTC = &phys; owner.pos = C3DVector(5, 6, 7);
    CStealthDef def = { 10.0f, 4.0f, 2.0f, 1.0f };
    CTCStealth s;
    s.ParentThing = &owner; s.Sneaking = false; s.PDef = &def; s.DistanceCanBeHeardFrom = 99.0f;
    g_noise.m_flags91 = 0; g_noise.passedTo = 0;
    // still: silent, no noise
    s.UpdateHearability();
    CHECK(s.DistanceCanBeHeardFrom == 0.0f && g_noises == 0);
    // running (|v| = 5 > 3)
    phys.Position = C3DVector(3, 4, 0);
    s.UpdateHearability();
    CHECK(s.DistanceCanBeHeardFrom == 20.0f && g_noises == 1 && g_noiseDef == 77 && g_noiseRadius == 20.0f);
    CHECK(strcmp(g_lookedUp, "SIMPLE_NOISE") == 0 && g_noisePos.X == 5 && g_noisePos.Z == 7 && g_noise.passedTo == &owner);
    // walking (|v| = 2: between 1 and 3)
    phys.Position = C3DVector(0, 0, 2);
    s.UpdateHearability();
    CHECK(s.DistanceCanBeHeardFrom == 10.0f && g_noises == 2);
    // creeping (|v| = 0.5)
    phys.Position = C3DVector(0, 0.5f, 0);
    s.UpdateHearability();
    CHECK(s.DistanceCanBeHeardFrom == 5.0f && g_noises == 3);
    // a dead noise is not handed the owner
    g_noise.m_flags91 = 1; g_noise.passedTo = 0;
    s.UpdateHearability();
    CHECK(g_noises == 4 && g_noise.passedTo == 0);
    // sneaking or no def: silent
    s.Sneaking = true;
    s.UpdateHearability();
    CHECK(s.DistanceCanBeHeardFrom == 0.0f && g_noises == 4);
    s.Sneaking = false; s.PDef = 0;
    s.UpdateHearability();
    CHECK(s.DistanceCanBeHeardFrom == 0.0f && g_noises == 4);
    printf("007479b0_TEST PASS\n");
    return 0;
}
