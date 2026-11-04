// Behaviour test for 0x00877720 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
struct Thing { float X, Y; int Class; };
static void GetStandardThings(std::vector<const Thing*>& out, const std::vector<Thing>& world, float px, float py, float radius) {
    out.clear();
    const int classes[2] = { 3, 5 };
    for (int c = 0; c < 2; ++c) for (size_t i = 0; i < world.size(); ++i) {
        float dx = world[i].X - px, dy = world[i].Y - py;
        if (world[i].Class == classes[c] && dx * dx + dy * dy <= radius * radius) out.push_back(&world[i]);
    }
}
int main() {
    std::vector<Thing> w; Thing a = { 1, 1, 3 }, b = { 2, 0, 5 }, c = { 50, 50, 3 }, d = { 0, 1, 4 };
    w.push_back(a); w.push_back(b); w.push_back(c); w.push_back(d);
    std::vector<const Thing*> out; out.push_back(&w[3]); GetStandardThings(out, w, 0, 0, 5);
    bool ok = out.size() == 2 && out[0] == &w[0] && out[1] == &w[1];
    std::printf(ok ? "00877720_TEST PASS\n" : "00877720_TEST FAIL\n");
    return ok ? 0 : 1;
}
