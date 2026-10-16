#include <cstdio>
// Behaviour model of CTCPhysicsBase::GetPhysicsStateFlags (0x006AFBF0, vtable slot 88)
static bool g_bPhysicsStateFlagsEnabled;
struct SPhysicsStateFlags { bool IsOnMesh, IsOnGround, Moving, GetCollidesWithThings, GetInteractsWithThings, IsImmoveable; };
struct CTCPhysicsBase {
    bool Moving, onMesh, onGround, collides, interacts, immoveable;
    virtual bool IsOnMesh() { return onMesh; } virtual bool IsOnGround() { return onGround; }
    virtual bool GetCollidesWithThings() { return collides; } virtual bool GetInteractsWithThings() { return interacts; }
    virtual bool IsImmoveable() { return immoveable; }
    virtual ~CTCPhysicsBase() {}
    bool GetPhysicsStateFlags(SPhysicsStateFlags* out) {
        if (g_bPhysicsStateFlagsEnabled) {
            out->Moving = Moving; out->IsOnGround = IsOnGround(); out->IsOnMesh = IsOnMesh();
            out->GetCollidesWithThings = GetCollidesWithThings(); out->GetInteractsWithThings = GetInteractsWithThings(); out->IsImmoveable = IsImmoveable();
            return true;
        }
        return false;
    } };
int main()
{
    CTCPhysicsBase p; p.Moving = true; p.onMesh = false; p.onGround = true; p.collides = true; p.interacts = false; p.immoveable = true;
    SPhysicsStateFlags f = { 0, 0, 0, 0, 0, 0 };
    g_bPhysicsStateFlagsEnabled = false;
    if (p.GetPhysicsStateFlags(&f) || f.Moving) { std::printf("006afbf0_TEST FAIL\n"); return 1; }
    g_bPhysicsStateFlagsEnabled = true;
    if (!p.GetPhysicsStateFlags(&f) || !f.Moving || f.IsOnMesh || !f.IsOnGround || !f.GetCollidesWithThings || f.GetInteractsWithThings || !f.IsImmoveable)
    { std::printf("006afbf0_TEST FAIL\n"); return 1; }
    std::printf("006afbf0_TEST PASS\n");
    return 0;
}
