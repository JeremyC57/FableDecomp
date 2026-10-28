// Test: CIsUsableIciclePoint::operator() -- a point is usable when every icicle's 2D (X/Y) offset is at least IciclesLimit away
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct CIcicleData { long EffectTimer; long TrapTimer; C3DVector Position; };
struct CIsUsableIciclePoint {
    const CIcicleData* first; const CIcicleData* last; float IciclesLimit;
    bool operator()(const C3DVector& point) const
    {
        for (const CIcicleData* it = first; it != last; ++it) {
            float dx = it->Position.X - point.X, dy = it->Position.Y - point.Y;
            if (dy * dy + dx * dx < IciclesLimit * IciclesLimit) return false;
        }
        return true;
    }
};
int main()
{
    CIcicleData d[2] = { { 0, 0, { 0.0f, 0.0f, 0.0f } }, { 0, 0, { 10.0f, 0.0f, 0.0f } } };
    CIsUsableIciclePoint f = { d, d + 2, 3.0f };
    C3DVector far_ = { 5.0f, 0.0f, 100.0f }, near_ = { 9.0f, 1.0f, 0.0f }, zfar = { 0.0f, 2.0f, 50.0f };
    if (!f(far_) || f(near_) || f(zfar)) { std::printf("008e2e40_TEST FAIL\n"); return 1; }
    CIsUsableIciclePoint e = { d, d, 3.0f };
    if (!e(near_)) { std::printf("008e2e40_TEST FAIL\n"); return 1; }
    std::printf("008e2e40_TEST PASS\n");
    return 0;
}