// Behaviour test for 0x0089EF90 CGameScriptInterface::GetNearestEnabledDiggingSpot: nearest enabled spot by truncated 2D squared distance (first wins ties), empty handle when none are enabled.
#include <cstdio>
struct Spot { int id; float x, y, z; bool enabled; };
static int Nearest(const Spot* spots, int n, float fx, float fy)
{
    if (n == 0) return -1;
    int nearestDistance = -1;
    bool found = false;
    int nearest = 0;
    for (int i = 0; i < n; ++i) {
        if (spots[i].enabled) {
            float dx = spots[i].x - fx;
            float dy = spots[i].y - fy;
            float distance = dx * dx + dy * dy;
            if (nearestDistance == -1 || distance < nearestDistance) {
                nearestDistance = (int)distance;
                nearest = i;
            }
            found = true;
        }
    }
    if (!found) return -1;
    return spots[nearest].id;
}
int main()
{
    int bad = 0;
    Spot s[4] = { { 10, 5, 5, 0, true }, { 11, 1, 1, 9, false }, { 12, 2, 0, 0, true }, { 13, 2.2f, 0, 0, true } };
    if (Nearest(s, 0, 0, 0) != -1) ++bad;
    if (Nearest(s, 4, 0, 0) != 12) ++bad;          // 4 vs 50; disabled 11 ignored
    if (Nearest(s + 3, 1, 0, 0) != 13) ++bad;
    if (Nearest(s + 2, 2, 0, 0) != 12) ++bad;      // 4 then 4.84 -> truncated 4, not < 4
    Spot d[2] = { { 1, 0, 0, 0, false }, { 2, 1, 1, 1, false } };
    if (Nearest(d, 2, 0, 0) != -1) ++bad;
    Spot t[2] = { { 1, 3, 0, 0, true }, { 2, 3.1f, 0, 0, true } };  // 9 then 9.61 truncated: 9.61 < 9? no
    if (Nearest(t, 2, 0, 0) != 1) ++bad;
    if (bad) { std::printf("0089ef90_TEST FAIL %d\n", bad); return 1; }
    std::printf("0089ef90_TEST PASS\n");
    return 0;
}