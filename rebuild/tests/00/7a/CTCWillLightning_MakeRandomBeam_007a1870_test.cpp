// Behaviour test for CTCWillLightning::MakeRandomBeam (0x007A1870): clears BeamsInitialised, builds a beam from the
// caster with the beam timings and def, starts it at the caster's forward, rotates it by a random yaw (in turns of
// 1/360) about forward x up and by a random pitch about up, scales it by the area-effect range, offsets it by the
// caster position, flags it temporary/main with the caster's hand, activates it and appends it to the beam list.
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    void Rotate(const C3DVector& axis, float angle);
};
static C3DVector g_axes[2]; static float g_angles[2]; static int g_rot = 0;
void C3DVector::Rotate(const C3DVector& axis, float angle) { g_axes[g_rot] = axis; g_angles[g_rot] = angle; ++g_rot; }

inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    const float* pa = &a.X; const float* pb = &b.X;
    return C3DVector((float)(double)(pb[2] * pa[1] - pa[2] * pb[1]), (float)(double)(*(&b.Y - 1) * pa[2] - pa[0] * pb[2]),
                     (float)(double)(pb[1] * pa[0] - pa[1] * *(&b.Y - 1)));
}
class CRightHandedSet { public: C3DVector Up, Forward; };
class CFloatRange { public: float Min, Max; };
struct CLightningBeamDef { CFloatRange RandomBeamYawRange, RandomBeamPitchRange; };

static float g_randoms[2] = { 90.0f, 45.0f }; static int g_nRand = 0;
static float GFFloatRandom(float range) { (void)range; return g_randoms[g_nRand++]; }

class CThing { public:
    CRightHandedSet Set; C3DVector Position;
    const CRightHandedSet& GetRHSet() const { return Set; }
    const C3DVector& GetPosition() const { return Position; }
};
struct CLightningBeam {
    CThing* Source; float ActivationTime, DeactivationTime; CLightningBeamDef* PDef; C3DVector Destination;
    float Life; bool Temporary, MainBeam, RightHand; int Activated;
    CLightningBeam(CThing* s) : Source(s), Life(0), Temporary(false), MainBeam(false), RightHand(false), Activated(0) { }
    void Activate() { ++Activated; }
};
class CTCWillLightning { public:
    CThing* Player; CLightningBeamDef* m_lspDef; CLightningBeam* m_lbBeams[4]; int m_nBeams;
    bool BeamsInitialised, LeftHand;
    float m_fAreaEffect, m_fActivationTime, m_fDeActivationTime, m_fTemporaryBeamLife;
    bool IsRightHanded() const { return !LeftHand; }
    float GetRandomInRange(const CFloatRange& r) { return r.Min + GFFloatRandom(r.Max - r.Min); }
    void MakeRandomBeam(bool)
    {
        BeamsInitialised = false;
        CRightHandedSet set = Player->GetRHSet();
        CLightningBeam* beam = new CLightningBeam(Player);
        beam->ActivationTime = m_fActivationTime;
        beam->DeactivationTime = m_fDeActivationTime;
        beam->PDef = m_lspDef;
        beam->Destination = set.Forward;
        float yaw = GetRandomInRange(m_lspDef->RandomBeamYawRange);
        C3DVector side = CrossProduct(set.Forward, set.Up);
        beam->Destination.Rotate(side, yaw * (1.0f / 360.0f));
        float pitch = GetRandomInRange(m_lspDef->RandomBeamPitchRange);
        beam->Destination.Rotate(set.Up, pitch * (1.0f / 360.0f));
        C3DVector& destination = beam->Destination;
        destination *= m_fAreaEffect;
        destination += Player->GetPosition();
        beam->Life = m_fTemporaryBeamLife;
        beam->Temporary = true;
        beam->MainBeam = true;
        beam->RightHand = IsRightHanded();
        beam->Activate();
        m_lbBeams[m_nBeams++] = beam;
    }
};

int main()
{
    CThing player;
    player.Set.Up = C3DVector(0.0f, 0.0f, 1.0f); player.Set.Forward = C3DVector(0.0f, 1.0f, 0.0f);
    player.Position = C3DVector(10.0f, 20.0f, 30.0f);
    CLightningBeamDef def; def.RandomBeamYawRange.Min = 0.0f; def.RandomBeamYawRange.Max = 360.0f;
    def.RandomBeamPitchRange.Min = -90.0f; def.RandomBeamPitchRange.Max = 90.0f;
    CTCWillLightning w; w.Player = &player; w.m_lspDef = &def; w.m_nBeams = 0; w.BeamsInitialised = true; w.LeftHand = false;
    w.m_fAreaEffect = 4.0f; w.m_fActivationTime = 0.5f; w.m_fDeActivationTime = 2.0f; w.m_fTemporaryBeamLife = 1.5f;
    w.MakeRandomBeam(false);
    CLightningBeam* b = w.m_lbBeams[0];
    bool ok = !w.BeamsInitialised && w.m_nBeams == 1 && b->Source == &player && b->PDef == &def
        && b->ActivationTime == 0.5f && b->DeactivationTime == 2.0f && b->Life == 1.5f
        && b->Temporary && b->MainBeam && b->RightHand && b->Activated == 1 && g_rot == 2
        // side = forward x up = (1,0,0); yaw 90/360 = 0.25 turns; pitch -90+45 = -45 -> -0.125 turns about up
        && g_axes[0].X == 1.0f && g_axes[0].Y == 0.0f && g_axes[0].Z == 0.0f && g_angles[0] == 0.25f
        && g_axes[1].Z == 1.0f && g_angles[1] == -0.125f
        // stub rotations leave forward; scaled by 4 and offset by the position
        && b->Destination.X == 10.0f && b->Destination.Y == 24.0f && b->Destination.Z == 30.0f;
    if (ok) printf("007a1870_TEST PASS\n"); else printf("007a1870_TEST FAIL\n");
    delete b;
    return ok ? 0 : 1;
}