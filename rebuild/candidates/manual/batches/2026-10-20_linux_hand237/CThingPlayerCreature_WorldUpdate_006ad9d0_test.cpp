// Behaviour test for CThingPlayerCreature::WorldUpdate (0x006AD9D0): movement vector from the last position only once a
// last position exists, look/trail components driven when present, the collision record cleared, result false.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
static int g_calls[6];
struct CTCLook { bool TrackNearestInterestingThing(bool f) { if (f) ++g_calls[0]; return true; } };
struct CTCWeaponTrail { void UpdateWeaponTrails() { ++g_calls[1]; } };
struct CThingPlayerCreature {
    C3DVector Pos, LastPos, MovementVector; unsigned char m_updateFlags;
    CTCLook* Look; CTCWeaponTrail* Trail; float AnimStep;
    bool CollidedWithThing; C3DVector LastThingCollisionNormal, LastThingCollisionPosition;
    const C3DVector& GetPosition() const { return Pos; }
    const C3DVector& GetLastPosition() const { return LastPos; }
    void UpdateAction() { ++g_calls[2]; }
    bool BaseWorldUpdate() { ++g_calls[3]; return true; }
    void UpdatePlayerControl() { ++g_calls[4]; }
    void UpdateAnimation(float s) { AnimStep = s; ++g_calls[5]; }
    bool WorldUpdate()
    {
        UpdateAction();
        BaseWorldUpdate();
        if (m_updateFlags & 2)
            MovementVector = GetPosition() - GetLastPosition();
        m_updateFlags |= 2;
        if (Look)
            Look->TrackNearestInterestingThing(true);
        UpdatePlayerControl();
        UpdateAnimation(0.0f);
        CollidedWithThing = false;
        LastThingCollisionNormal.X = LastThingCollisionNormal.Y = LastThingCollisionNormal.Z = 0.0f;
        LastThingCollisionPosition.X = LastThingCollisionPosition.Y = LastThingCollisionPosition.Z = 0.0f;
        if (Trail)
            Trail->UpdateWeaponTrails();
        return false;
    }
};

int main()
{
    CTCLook look;
    CThingPlayerCreature c;
    c.Pos = C3DVector(5, 6, 7); c.LastPos = C3DVector(1, 2, 3); c.MovementVector = C3DVector(9, 9, 9);
    c.m_updateFlags = 0; c.Look = &look; c.Trail = 0; c.AnimStep = 1.0f; c.CollidedWithThing = true;
    c.LastThingCollisionNormal = C3DVector(1, 1, 1); c.LastThingCollisionPosition = C3DVector(2, 2, 2);
    bool ok = !c.WorldUpdate() && c.MovementVector.X == 9 && (c.m_updateFlags & 2) && g_calls[0] == 1 && g_calls[1] == 0;
    ok = ok && c.AnimStep == 0.0f && !c.CollidedWithThing && c.LastThingCollisionNormal.Y == 0 && c.LastThingCollisionPosition.Z == 0;
    ok = ok && !c.WorldUpdate() && c.MovementVector.X == 4 && c.MovementVector.Y == 4 && c.MovementVector.Z == 4;
    ok = ok && g_calls[2] == 2 && g_calls[3] == 2 && g_calls[4] == 2 && g_calls[5] == 2;
    if (!ok) { std::printf("006ad9d0_TEST FAIL\n"); return 1; }
    std::printf("006ad9d0_TEST PASS\n");
    return 0;
}
