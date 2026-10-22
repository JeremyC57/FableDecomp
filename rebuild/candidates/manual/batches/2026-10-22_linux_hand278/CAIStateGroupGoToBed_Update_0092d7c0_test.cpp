// Behaviour test for CAIStateGroup_GoToBed::Update (0x0092D7C0): model copy with scripted stubs.
#include <cstdio>
#include <cmath>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    float Magnitude() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCSleep { bool IsReadyToSleep; };

static bool g_hasAnim, g_hasSleepTC, g_bedAlive;
static long g_playResult, g_type, g_lookResult, g_moves[8], g_mi, g_movement, g_terminated;
static C3DVector g_lookPos;
static CTCSleep g_sleep;

struct CThing {
    C3DVector pos;
    unsigned char m_flags91;
    long GetCreatureType() const { return g_type; }
    const C3DVector& GetPosition() const { return pos; }
    bool HasTC(long t) const { return t == 0xd4 && g_hasSleepTC; }
    CTCSleep* GetTCSleep() { return &g_sleep; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return g_bedAlive ? m_pItem : 0; }
};
struct CAIStateGroup_GoToBed {
    CThing* creature;
    CBaseIntelligentPointer PBed;
    C3DVector BedPos;
    CRightHandedSet Rhset;
    bool own, bandit;
    CThing* GetCreature() const { return creature; }
    bool FindFreeBed(bool) { return own; }
    bool FindFreeBanditBed() { return bandit; }
    bool CreatureHasAnim(const char*) const { return g_hasAnim; }
    long PlayAnimation(const char*) { return g_playResult; }
    long AIAction_MoveToPos(long, C3DVector, bool* arrived, long movement, bool, bool, CThing*, bool, float)
    {
        *arrived = false; g_movement = movement; return g_moves[g_mi++];
    }
    long AIAction_LookAtPos(long, C3DVector pos, long, float) { g_lookPos = pos; return g_lookResult; }
    void Terminate() { ++g_terminated; }
    void Update()
    {
        if (FindFreeBed(false) || FindFreeBanditBed()) {
            bool arrived;
            bool failed = CreatureHasAnim("STANDARD_YAWN") && PlayAnimation("STANDARD_YAWN") != 2;
            if (!failed) {
                long movement = 3;
                if (GetCreature()->GetCreatureType() == 1 && (BedPos - GetCreature()->GetPosition()).Magnitude() > 15.0f)
                    movement = 5;
                arrived = false;
                long result;
                do {
                    result = AIAction_MoveToPos(2, BedPos, &arrived, movement, true, true, 0, true, 0.0f);
                } while (result == 4);
                if (result == 2 && PBed.GetPItem() != 0 && !(PBed.GetPItem()->m_flags91 & 1)
                    && AIAction_LookAtPos(1, GetCreature()->GetPosition() + Rhset.Forward, 0x11, 0.01f) == 2
                    && GetCreature()->HasTC(0xd4))
                    GetCreature()->GetTCSleep()->IsReadyToSleep = true;
            }
        }
        Terminate();
    }
};

static int fail() { std::printf("0092d7c0_TEST FAIL\n"); return 1; }

int main()
{
    CThing me; me.pos = C3DVector(0, 0, 0); me.m_flags91 = 0;
    CThing bed; bed.m_flags91 = 0;
    CAIStateGroup_GoToBed g;
    g.creature = &me; g.PBed.m_pItem = &bed; g.BedPos = C3DVector(20, 0, 0);
    g.Rhset.Forward = C3DVector(0, 1, 0); g.own = false; g.bandit = true;
    g_hasAnim = true; g_playResult = 2; g_type = 1; g_bedAlive = true; g_hasSleepTC = true; g_lookResult = 2;
    g_moves[0] = 4; g_moves[1] = 2; g_mi = 0; g_sleep.IsReadyToSleep = false;
    g.Update();
    if (!g_sleep.IsReadyToSleep || g_movement != 5 || g_mi != 2 || g_terminated != 1 || g_lookPos.Y != 1.0f) return fail();
    // yawn fails: no walk, still terminates
    g_playResult = 3; g_mi = 0; g_sleep.IsReadyToSleep = false;
    g.Update();
    if (g_mi != 0 || g_sleep.IsReadyToSleep || g_terminated != 2) return fail();
    // close bed, not human: walk; dead bed: no sleep flag
    g_playResult = 2; g.BedPos = C3DVector(3, 0, 0); g_type = 0; g_moves[0] = 2; g_mi = 0; g_bedAlive = false;
    g.Update();
    if (g_movement != 3 || g_sleep.IsReadyToSleep) return fail();
    // no bed at all
    g.bandit = false; g_mi = 0;
    g.Update();
    if (g_mi != 0 || g_terminated != 4) return fail();
    std::printf("0092d7c0_TEST PASS\n");
    return 0;
}
