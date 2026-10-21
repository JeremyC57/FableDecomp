// Behaviour test for CGameScriptInterface::GetFurthestWithDefName @ 0x0089AA70: empty list gives an empty handle,
// otherwise the thing with the largest truncated squared 2D distance from `from`, first one on ties.
#include <cstdio>
#include <vector>
struct Thing { int id; float x, y, z; };
static Thing Furthest(const Thing& from, const std::vector<Thing>& things)
{
    if (things.empty()) { Thing none = { 0, 0, 0, 0 }; return none; }
    int furthestDistance = 0;
    size_t furthest = 0;
    for (size_t i = 0; i < things.size(); ++i) {
        float dx = things[i].x - from.x;
        float dy = things[i].y - from.y;
        float distance = dx * dx + dy * dy;
        if (distance > furthestDistance) { furthestDistance = (int)distance; furthest = i; }
    }
    return things[furthest];
}
int main()
{
    Thing from = { 9, 0, 0, 0 };
    std::vector<Thing> v;
    if (Furthest(from, v).id != 0) { std::printf("0089aa70_TEST FAIL\n"); return 1; }
    Thing a = { 1, 1, 1, 50 }, b = { 2, 3, 0, 0 }, c = { 3, 0, 3, 0 }, d = { 4, 0.5f, 0, 0 };
    v.push_back(a); v.push_back(b); v.push_back(c); v.push_back(d);
    if (Furthest(from, v).id != 2) { std::printf("0089aa70_TEST FAIL\n"); return 1; }  // z ignored, tie keeps first
    std::vector<Thing> w; w.push_back(d);
    if (Furthest(from, w).id != 4) { std::printf("0089aa70_TEST FAIL\n"); return 1; }  // 0.25 truncates to 0: still the first
    std::vector<Thing> e; Thing s = { 5, 0, 0, 0 }; e.push_back(s);
    if (Furthest(from, e).id != 5) { std::printf("0089aa70_TEST FAIL\n"); return 1; }
    std::printf("0089aa70_TEST PASS\n");
    return 0;
}