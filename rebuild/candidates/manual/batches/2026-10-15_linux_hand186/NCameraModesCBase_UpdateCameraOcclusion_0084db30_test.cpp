#include <cstdio>
struct V { float X, Y, Z; };
struct Wall { float x; };   // a wall at X = x blocks lines crossing it
static int g_checks = 0;
struct Mode {
    bool enabled; bool Occluded; V focal; Wall wall; int maxHits;
    bool Check(const V& from, const V& to, V& hit)
    {
        ++g_checks;
        if (maxHits-- <= 0) return false;
        if ((from.X - wall.x) * (to.X - wall.x) >= 0) return false;
        hit.X = wall.x; hit.Y = from.Y; hit.Z = from.Z; return true;
    }
    void Update(V& cam)
    {
        Occluded = false;
        if (!enabled) return;
        V from = focal, to = cam, hit;
        if (Check(from, to, hit)) { cam = hit; Occluded = true; if (Check(from, cam, hit)) cam = hit; }
    }
};
int main()
{
    Mode m = { true, true, { 0, 0, 1 }, { 5 }, 2 };
    V cam = { 10, 0, 1 };
    m.Update(cam);
    bool ok1 = m.Occluded && cam.X == 5 && g_checks == 2;
    Mode n = { false, true, { 0, 0, 1 }, { 5 }, 2 };
    V cam2 = { 10, 0, 1 };
    n.Update(cam2);
    bool ok2 = !n.Occluded && cam2.X == 10 && g_checks == 2;
    Mode o = { true, true, { 0, 0, 1 }, { 50 }, 2 };
    V cam3 = { 10, 0, 1 }; o.Update(cam3);
    bool ok3 = !o.Occluded && cam3.X == 10;
    if (!ok1 || !ok2 || !ok3) { std::printf("0084db30_TEST FAIL\n"); return 1; }
    std::printf("0084db30_TEST PASS\n");
    return 0;
}
