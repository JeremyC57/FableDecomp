// Behaviour test for NHitLocations::CPhysicalPrimitiveCylinder::GetClosestPointToPos2D (0x00816410): for an upright cylinder the
// footprint is the circle of its radius, so the closest point to an outside position lies on that circle along the line to the
// centre, and the XY normal is the unit direction from the centre to it.
#include <cmath>
#include <cstdio>

static void Closest(float cx, float cy, float radius, float px, float py, float& qx, float& qy, float& nx, float& ny)
{
    // sample the ellipse centre + major*sin(t) + minor*cos(t) (major = (r,0), minor = (0,r)) and keep the nearest point
    const double TWO_PI = 6.283185482025146484375;
    float best = 1e30f;
    for (int i = 0; i < 3600; ++i) {
        double t = i / 3600.0;
        float x = cx + radius * (float)std::sin(t * TWO_PI), y = cy + radius * (float)std::cos(t * TWO_PI);
        float d = (x - px) * (x - px) + (y - py) * (y - py);
        if (d < best) { best = d; qx = x; qy = y; }
    }
    nx = qx - cx; ny = qy - cy;
    float m = (float)std::sqrt(nx * nx + ny * ny);
    if (m > 1e-6f) { nx /= m; ny /= m; }
}

int main()
{
    float qx, qy, nx, ny;
    Closest(1.0f, 2.0f, 0.5f, 5.0f, 2.0f, qx, qy, nx, ny);
    bool ok = std::fabs(qx - 1.5f) < 1e-3f && std::fabs(qy - 2.0f) < 1e-3f && std::fabs(nx - 1.0f) < 1e-3f && std::fabs(ny) < 1e-3f;
    Closest(0.0f, 0.0f, 2.0f, 0.0f, -9.0f, qx, qy, nx, ny);
    ok = ok && std::fabs(qx) < 1e-2f && std::fabs(qy + 2.0f) < 1e-3f && std::fabs(ny + 1.0f) < 1e-3f;
    if (!ok) { std::printf("00816410_TEST FAIL\n"); return 1; }
    std::printf("00816410_TEST PASS\n");
    return 0;
}
