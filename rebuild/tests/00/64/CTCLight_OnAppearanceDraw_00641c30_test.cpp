// Behaviour test for CTCLight::OnAppearanceDraw (0x00641C30): a model of the draw loop -- lights at every light helper that resolves,
// flares at every flare helper, and the single fallback light at the thing (class 2: interpolated position + facing*0.35, Z+1;
// otherwise the thing's position) only when no helper light was drawn; false when the appearance is not loaded, true otherwise.
#include <cstdio>
#include <vector>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Appearance {
    bool loaded; int lightHelpers, flareHelpers; std::vector<int> missing;   // helper indices that fail to resolve
    bool IsResourcesLoaded(bool) const { return loaded; }
    long GetNoHelperPoints(int kind) const { return kind == 0 ? lightHelpers : flareHelpers; }
    bool GetHelperPoint(int kind, long i, V3& p) const {
        for (unsigned k = 0; k < missing.size(); ++k) if (missing[k] == i + kind * 100) return false;
        p = V3((float)i, (float)kind, 0.0f); return true; }
};
struct Light {
    bool Active; int thingType; V3 thingPos, physPos, physOld, facing;
    std::vector<V3> lights; std::vector<V3> flares;
    void DrawLight(long, const V3& p) { lights.push_back(p); }
    void DrawFlare(long, const V3& p) { flares.push_back(p); }
    bool OnAppearanceDraw(const Appearance& a, float t) {
        if (Active) {
            if (!a.IsResourcesLoaded(true)) return false;
            long drawn = 0; long n = a.GetNoHelperPoints(0); long i; V3 hp;
            for (i = 0; i < n; i++) if (a.GetHelperPoint(0, i, hp)) { DrawLight(i, hp); drawn++; }
            long nf = a.GetNoHelperPoints(1);
            for (i = 0; i < nf; i++) if (a.GetHelperPoint(1, i, hp)) DrawFlare(i, hp);
            if (drawn == 0) {
                V3 pos;
                if (thingType == 2) {
                    V3 ip((physPos.X - physOld.X) * t + physOld.X, (physPos.Y - physOld.Y) * t + physOld.Y, (physPos.Z - physOld.Z) * t + physOld.Z);
                    pos = V3(ip.X + facing.X * 0.35f, ip.Y + facing.Y * 0.35f, ip.Z + facing.Z * 0.35f + 1.0f);
                } else pos = thingPos;
                DrawLight(0, pos);
            }
        }
        return true;
    }
};
static int fail() { std::printf("00641c30_TEST FAIL\n"); return 1; }
int main()
{
    Appearance a; a.loaded = true; a.lightHelpers = 3; a.flareHelpers = 2; a.missing.push_back(1);
    Light l; l.Active = true; l.thingType = 2; l.thingPos = V3(5, 6, 7); l.physPos = V3(2, 0, 0); l.physOld = V3(0, 0, 0); l.facing = V3(0, 1, 0);
    if (!l.OnAppearanceDraw(a, 0.5f) || l.lights.size() != 2 || l.flares.size() != 2) return fail();
    a.lightHelpers = 0; l.lights.clear(); l.flares.clear();
    if (!l.OnAppearanceDraw(a, 0.5f) || l.lights.size() != 1) return fail();
    if (l.lights[0].X != 1.0f || l.lights[0].Y != 0.35f || l.lights[0].Z != 1.0f) return fail();
    l.thingType = 3; l.lights.clear();
    if (!l.OnAppearanceDraw(a, 0.5f) || l.lights.size() != 1 || l.lights[0].X != 5.0f || l.lights[0].Z != 7.0f) return fail();
    a.loaded = false; if (l.OnAppearanceDraw(a, 0.5f)) return fail();
    l.Active = false; l.lights.clear(); if (!l.OnAppearanceDraw(a, 0.5f) || !l.lights.empty()) return fail();
    std::printf("00641c30_TEST PASS\n");
    return 0;
}