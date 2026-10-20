// Behaviour test for 007e2f00: body copied from src/007e2f00.cpp (CTCWobble::FrameUpdate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_rotate = 0; static float g_rotAngle = 0; static float g_axisX = 0;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector& operator=(const C3DVector& other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }
    void Rotate(const C3DVector& axis, float angle) { ++g_rotate; g_rotAngle = angle; g_axisX = axis.X; X += 100.0f; }
};
struct CRightHandedSet {
    C3DVector Up;          // +0x00
    C3DVector Forward;     // +0x0c
};
static int g_setFacing = 0; static CRightHandedSet g_facing;
struct CTCPhysicsBase {
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& rhSet) { ++g_setFacing; g_facing = rhSet; }
};
static int g_removeTC = 0; static long g_removeType = 0;
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;
    void RemoveTC(long interfaceType) { ++g_removeTC; g_removeType = interfaceType; }
};
struct CTCWobble {
    virtual void v00() { }
    CThing*         ParentThing;
    unsigned char   _pad_0x08[4];
    CRightHandedSet ParentRHSet;
    long            FramesOfWobbleRemaining;
    float           DirectionLastRotated;
    float           AngleOfWobble;
    void FrameUpdate();
};
void CTCWobble::FrameUpdate()
{
    if (FramesOfWobbleRemaining) {
        --FramesOfWobbleRemaining;
        CRightHandedSet rhSet = ParentRHSet;
        C3DVector up = rhSet.Up;
        up.Rotate(rhSet.Forward, -DirectionLastRotated * AngleOfWobble);
        rhSet.Up = up;
        ParentThing->PhysicsTC->SetFacingAnglesFromRHSet(rhSet);
        DirectionLastRotated = -DirectionLastRotated;
    } else {
        ParentThing->PhysicsTC->SetFacingAnglesFromRHSet(ParentRHSet);
        ParentThing->RemoveTC(0xf7);
    }
}
int main()
{
    CTCPhysicsBase phys; CThing thing; memset(&thing, 0, sizeof thing); thing.PhysicsTC = &phys;
    CTCWobble w; w.ParentThing = &thing;
    w.ParentRHSet.Up.X = 1; w.ParentRHSet.Up.Y = 2; w.ParentRHSet.Up.Z = 3;
    w.ParentRHSet.Forward.X = 7; w.ParentRHSet.Forward.Y = 8; w.ParentRHSet.Forward.Z = 9;
    w.FramesOfWobbleRemaining = 2; w.DirectionLastRotated = 1.0f; w.AngleOfWobble = 0.5f;
    w.FrameUpdate();
    CHECK(w.FramesOfWobbleRemaining == 1 && g_rotate == 1 && g_rotAngle == -0.5f && g_axisX == 7);
    CHECK(g_setFacing == 1 && g_facing.Up.X == 101.0f && g_facing.Up.Y == 2 && g_facing.Up.Z == 3 && g_facing.Forward.Z == 9);
    CHECK(w.ParentRHSet.Up.X == 1);                       // the stored set is untouched (copy rotated)
    CHECK(w.DirectionLastRotated == -1.0f && g_removeTC == 0);
    w.FrameUpdate();
    CHECK(w.FramesOfWobbleRemaining == 0 && g_rotate == 2 && g_rotAngle == 0.5f && w.DirectionLastRotated == 1.0f && g_setFacing == 2);
    w.FrameUpdate();                                      // no frames left: restore and remove
    CHECK(g_rotate == 2 && g_setFacing == 3 && g_facing.Up.X == 1 && g_removeTC == 1 && g_removeType == 0xf7);
    CHECK(w.FramesOfWobbleRemaining == 0 && w.DirectionLastRotated == 1.0f);
    if (fails == 0) printf("007e2f00_TEST PASS\n"); else printf("007e2f00_TEST FAIL (%d)\n", fails);
    return fails;
}