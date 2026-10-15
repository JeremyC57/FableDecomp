// Behaviour test for CTCDegradableBase::OnHit(const C3DVector&, const C3DVector&) @ 0x008ECC80: the hit frame is always
// stamped; OnParentHealthChanged(false) runs only when the component was available to hit (> 1 frame since the last hit).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
static long g_frame = 0;
static int g_healthCalls = 0;
static bool g_healthArg = true;
struct CWorld { static long GetWorldFrame() { return g_frame; } };
static CWorld g_world;
struct CTCDegradableBase {
    virtual void v00() { }
    unsigned char _base[0x8];
    long          LastWorldFrameHit;
    void*         PDefinition;
    void*         PCurrentDegradableInfo;
    void*         PParentThingPhysical;
    CWorld*       GetWorld() { return &g_world; }
    const CWorld* GetWorld() const { return &g_world; }
    bool IsAvailableToHit() const;
    void SetAvailabilityToHitFromFrame(long frame) { LastWorldFrameHit = frame; }
    void OnParentHealthChanged(bool fromHit) { ++g_healthCalls; g_healthArg = fromHit; }
    void OnHit(const C3DVector& position, const C3DVector& direction);
};
bool CTCDegradableBase::IsAvailableToHit() const
{
    bool available = false;
    if (GetWorld()->GetWorldFrame() - LastWorldFrameHit > 1)
        available = true;
    return available;
}
void CTCDegradableBase::OnHit(const C3DVector& position, const C3DVector& direction)
{
    long frame = GetWorld()->GetWorldFrame();
    if (IsAvailableToHit()) {
        SetAvailabilityToHitFromFrame(frame);
        OnParentHealthChanged(false);
    } else {
        SetAvailabilityToHitFromFrame(frame);
    }
}
int main()
{
    CTCDegradableBase d; d.LastWorldFrameHit = 10; C3DVector p = { 0, 0, 0 };
    g_frame = 11; d.OnHit(p, p);
    if (g_healthCalls != 0 || d.LastWorldFrameHit != 11) { std::printf("008ecc80_TEST FAIL\n"); return 1; }
    g_frame = 20; d.OnHit(p, p);
    if (g_healthCalls != 1 || g_healthArg != false || d.LastWorldFrameHit != 20) { std::printf("008ecc80_TEST FAIL\n"); return 1; }
    std::printf("008ecc80_TEST PASS\n");
    return 0;
}
