// Behaviour test for NShape::CShape::UpdateSelectedPoint (0x007621C0), g++ model of the parked source.
#include <cstdio>
#include <vector>
#include <algorithm>
struct V { float X, Y, Z; };
struct Shape { std::vector<V> points; std::vector<V*> selected; };
static bool UpdateSelectedPoint(Shape& s, const V& pos)
{
    bool any = false;
    for (V* p = &s.points[0]; p != &s.points[0] + s.points.size(); ++p) {
        float dx = p->X - pos.X, dy = p->Y - pos.Y;
        if (dy * dy + dx * dx < 0.0625f) {
            if (std::find(s.selected.begin(), s.selected.end(), p) == s.selected.end())
                s.selected.push_back(p);
            any = true;
        }
    }
    if (!any) s.selected.clear();
    return any;
}
int main()
{
    Shape s; V a = { 0, 0, 0 }, b = { 0.1f, 0.1f, 5 }, c = { 3, 3, 0 };
    s.points.push_back(a); s.points.push_back(b); s.points.push_back(c);
    V pos = { 0.05f, 0.05f, 0 };
    int ok = UpdateSelectedPoint(s, pos) && s.selected.size() == 2;
    ok &= UpdateSelectedPoint(s, pos) && s.selected.size() == 2;     // no duplicates
    V far = { 10, 10, 0 };
    ok &= !UpdateSelectedPoint(s, far) && s.selected.empty();
    V nearC = { 3.2f, 3.1f, 9 };
    ok &= UpdateSelectedPoint(s, nearC) && s.selected.size() == 1 && s.selected[0] == &s.points[2];
    std::printf(ok ? "007621c0_TEST PASS\n" : "007621c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
