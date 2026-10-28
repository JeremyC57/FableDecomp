extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x0084D0A0 NCameraModes::CBase::Die: the mode's slot-22 hook always runs; only on a single cut are the
// FOV, roll angle and observed-position Z offset reset to their defaults.
class CGameCameraManager { public: bool SingleCut; };
struct CBase {
    int hooks; float DefaultFOV, DefaultRollAngle, DefaultObservedPositionZOffset;
    CGameCameraManager* CameraManager; float CameraFOV, RollAngle, ObservedPositionZOffset;
    void OnDie() { ++hooks; }
    void Die() { OnDie(); if (CameraManager->SingleCut) { CameraFOV = DefaultFOV; RollAngle = DefaultRollAngle; ObservedPositionZOffset = DefaultObservedPositionZOffset; } }
};
int main()
{
    CGameCameraManager m = { false };
    CBase b = { 0, 1.0f, 2.0f, 3.0f, &m, 9.0f, 8.0f, 7.0f };
    b.Die(); CHECK(b.hooks == 1 && b.CameraFOV == 9.0f && b.RollAngle == 8.0f && b.ObservedPositionZOffset == 7.0f);
    m.SingleCut = true;
    b.Die(); CHECK(b.hooks == 2 && b.CameraFOV == 1.0f && b.RollAngle == 2.0f && b.ObservedPositionZOffset == 3.0f);
    if (fails == 0) printf("0084d0a0_TEST PASS\n"); else printf("0084d0a0_TEST FAIL\n");
    return fails;
}