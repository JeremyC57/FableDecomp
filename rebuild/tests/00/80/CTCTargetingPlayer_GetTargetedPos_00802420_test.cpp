#include <cstdio>
// Behaviour test for CTCTargetingPlayer::GetTargetedPos (0x00802420): the body of src/ over stand-ins -- first-person
// interpolation of position plus offset, the live target's fire point otherwise, false with no live target.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
};
class CPlayer { public: bool firstPerson; bool IsFirstPersonTargeting() { return firstPerson; } };
class CWorld { public: int dummy; };
static CWorld g_world;
class CThing {
public:
    unsigned char m_flags91;
    CPlayer* player;
    float fireZ;
    CPlayer* GetPlayer() { return player; }
};
static const CWorld* g_gotWorld; static float g_gotT;
static C3DVector GetTargetFirePoint(const CWorld* world, const CThing* target, float t)
{
    g_gotWorld = world; g_gotT = t;
    return C3DVector(1, 2, target->fireZ);
}
class CTCTargetingPlayer { public:
    CThing* target;
    CThing* GetTargetedThing() { return target; }
    CThing* ParentThing;
    C3DVector TargetedPos, OldTargetedPos, TargetedPosOffset3D, OldTargetedPosOffset3D;
    const CWorld* GetWorld() const { return &g_world; }
    bool GetTargetedPos(C3DVector& pos, float t);
};

bool CTCTargetingPlayer::GetTargetedPos(C3DVector& pos, float t)
{
    CThing* target = GetTargetedThing();
    if (ParentThing->GetPlayer()->IsFirstPersonTargeting()) {
        pos = (TargetedPos - OldTargetedPos) * t + OldTargetedPos;
        pos += (TargetedPosOffset3D - OldTargetedPosOffset3D) * t + OldTargetedPosOffset3D;
        return true;
    }
    if (target && !(target->m_flags91 & 1)) {
        pos = GetTargetFirePoint(GetWorld(), target, t);
        return true;
    }
    return false;
}

static int fail(int line) { printf("00802420_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CPlayer player = { true };
    CThing owner = { 0, &player, 0 };
    CThing target = { 0, 0, 9.0f };
    CTCTargetingPlayer tp;
    tp.target = &target; tp.ParentThing = &owner;
    tp.OldTargetedPos = C3DVector(0, 0, 0); tp.TargetedPos = C3DVector(10, 20, 40);
    tp.OldTargetedPosOffset3D = C3DVector(1, 1, 1); tp.TargetedPosOffset3D = C3DVector(3, 5, 9);
    C3DVector pos(0, 0, 0);
    CHECK(tp.GetTargetedPos(pos, 0.5f));
    CHECK(pos.X == 7.0f && pos.Y == 13.0f && pos.Z == 25.0f);
    player.firstPerson = false;
    CHECK(tp.GetTargetedPos(pos, 0.25f));
    CHECK(pos.X == 1.0f && pos.Y == 2.0f && pos.Z == 9.0f && g_gotWorld == &g_world && g_gotT == 0.25f);
    target.m_flags91 = 1;
    pos = C3DVector(-1, -1, -1);
    CHECK(!tp.GetTargetedPos(pos, 0.25f) && pos.X == -1.0f);
    tp.target = 0;
    CHECK(!tp.GetTargetedPos(pos, 0.25f));
    printf("00802420_TEST PASS\n");
    return 0;
}