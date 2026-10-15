// Behaviour test for CTCDegradableBase::OnHitByShot(const C3DVector&, const C3DVector&, CThing&) @ 0x008EC810: the hit is forwarded
// to the virtual OnHit with the same two vectors (the override in a derived class receives it).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CThing { int id; };
static int g_hits = 0;
static const C3DVector* g_pos = 0;
static const C3DVector* g_dir = 0;
struct CTCDegradableBase {
    virtual void OnHitByMelee(const C3DVector& position, const C3DVector& direction, CThing& attacker);
    virtual void OnHitByShot(const C3DVector& position, const C3DVector& direction, CThing& attacker);
    virtual void OnHit(const C3DVector& position, const C3DVector& direction) { }
    virtual ~CTCDegradableBase() { }
    long LastWorldFrameHit;
};
void CTCDegradableBase::OnHitByMelee(const C3DVector& position, const C3DVector& direction, CThing& attacker) { OnHit(position, direction); }
void CTCDegradableBase::OnHitByShot(const C3DVector& position, const C3DVector& direction, CThing& attacker) { OnHit(position, direction); }
struct CRecorder : CTCDegradableBase {
    virtual void OnHit(const C3DVector& position, const C3DVector& direction) { ++g_hits; g_pos = &position; g_dir = &direction; }
};
int main()
{
    CRecorder r; C3DVector p = { 1.0f, 2.0f, 3.0f }, d = { 0.0f, 1.0f, 0.0f }; CThing who;
    CTCDegradableBase* base = &r;
    base->OnHitByShot(p, d, who);
    if (g_hits != 1 || g_pos != &p || g_dir != &d) { std::printf("008ec810_TEST FAIL\n"); return 1; }
    std::printf("008ec810_TEST PASS\n");
    return 0;
}
