// Behaviour test for 0x0063E590 CWorldMap::GetNavigationLayerAt: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct Thing { float x, y; long layer; bool nav; };
static long Layer(Thing* things, int n, float px, float py, const Thing* ignore, bool mapLayers, bool persistent, long mapLayer)
{
    if (mapLayers && !ignore && !persistent) return mapLayer;
    long best = 0;
    for (int i = 0; i < n; ++i) {
        Thing& t = things[i];
        if (&t == ignore || !t.nav) continue;
        float dx = t.x - px, dy = t.y - py;
        if (dx * dx + dy * dy <= 0.01f + 1.0f && t.layer > best) best = t.layer;
    }
    return best;
}
int main() {
    Thing t[2] = { { 0.0f, 0.0f, 2, true }, { 0.5f, 0.0f, 3, false } };
    int ok = Layer(t, 2, 0.0f, 0.0f, 0, true, false, 7) == 7 && Layer(t, 2, 0.0f, 0.0f, 0, false, false, 7) == 2
        && Layer(t, 2, 0.0f, 0.0f, &t[0], false, false, 7) == 0;
    std::printf(ok ? "0063e590_TEST PASS\n" : "0063e590_TEST FAIL\n");
    return ok ? 0 : 1;
}
