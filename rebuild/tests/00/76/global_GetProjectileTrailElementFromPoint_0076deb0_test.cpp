// Behaviour test for 0x0076DEB0 GetProjectileTrailElementFromPoint(point) -- point -/+ half of an eighth of the camera up
#include <cstdio>

struct V {
    float X, Y, Z;
};
struct Element { V first; V second; };

static Element GetElement(const V& point, const V& cameraUp)
{
    V offset = { cameraUp.X * 0.125f, cameraUp.Y * 0.125f, cameraUp.Z * 0.125f };
    V h = { offset.X * 0.5f, offset.Y * 0.5f, offset.Z * 0.5f };
    Element e;
    e.first.X = point.X - h.X; e.first.Y = point.Y - h.Y; e.first.Z = point.Z - h.Z;
    e.second.X = h.X + point.X; e.second.Y = h.Y + point.Y; e.second.Z = h.Z + point.Z;
    return e;
}

int main()
{
    V point = { 1.0f, 2.0f, 3.0f }, up = { 0.0f, 0.0f, 8.0f };
    Element e = GetElement(point, up);                   // half = (0, 0, 0.5)
    if (e.first.X != 1.0f || e.first.Y != 2.0f || e.first.Z != 2.5f) { std::printf("0076deb0_TEST FAIL\n"); return 1; }
    if (e.second.X != 1.0f || e.second.Y != 2.0f || e.second.Z != 3.5f) { std::printf("0076deb0_TEST FAIL\n"); return 1; }
    V tilted = { 16.0f, -16.0f, 0.0f };                  // half = (1, -1, 0)
    e = GetElement(point, tilted);
    if (e.first.X != 0.0f || e.first.Y != 3.0f || e.second.X != 2.0f || e.second.Y != 1.0f) { std::printf("0076deb0_TEST FAIL\n"); return 1; }
    std::printf("0076deb0_TEST PASS\n");
    return 0;
}