// Behaviour test for CTools::IsPosInViewingFrustrum (00875120).
#include <cstdio>
#include <cmath>
struct V { float x, y, z; V(float a, float b, float c) : x(a), y(b), z(c) { }
    V operator-(const V& o) const { return V(x - o.x, y - o.y, z - o.z); }
    void Normalise() { float l = (float)std::sqrt(x * x + y * y + z * z); if (l > 0) { x /= l; y /= l; z /= l; } } };
static long IsPosInViewingFrustrum(const V& pos, const V& lookAt, const V& eye, float fov)
{
    V view = lookAt - eye; view.Normalise();
    V dir = pos - eye; dir.Normalise();
    if (view.y * dir.y + view.x * dir.x + view.z * dir.z > std::cos(fov * 0.5f * (double)6.28318548f)) return 1;
    return 0;
}
int main()
{
    V eye(0, 0, 0), look(0, 1, 0);
    bool ok = IsPosInViewingFrustrum(V(0.1f, 5, 0), look, eye, 0.25f) == 1      // 90 degree cone, nearly straight ahead
        && IsPosInViewingFrustrum(V(5, 0.1f, 0), look, eye, 0.25f) == 0         // ~89 degrees off: outside the 45 degree half-angle
        && IsPosInViewingFrustrum(V(0, -5, 0), look, eye, 0.9f) == 0;
    if (!ok) { std::printf("00875120_TEST FAIL\n"); return 1; }
    std::printf("00875120_TEST PASS\n");
    return 0;
}
