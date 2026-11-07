// Behaviour test for 0x0065B142 CTCVillage::EnableVillagerDefTypes: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <vector>
struct Villager { long def; bool enabled; bool alive; bool hasNav; int created; };
static void Enable(std::vector<Villager>& d, bool enable, long def)
{
    for (size_t i = 0; i < d.size(); ++i) {
        Villager& v = d[i];
        if (v.def != def) continue;
        v.enabled = enable;
        if (!enable) { if (v.alive) v.alive = false; }
        else if (!v.alive) { v.created++; v.alive = true; }
    }
}
int main() {
    std::vector<Villager> d(3); Villager a = { 1, false, false, true, 0 }, b = { 2, true, true, false, 0 }, c = { 1, true, true, false, 0 };
    d[0] = a; d[1] = b; d[2] = c;
    Enable(d, true, 1);
    int ok = d[0].enabled && d[0].alive && d[0].created == 1 && d[2].created == 0 && d[1].alive;
    Enable(d, false, 1); ok &= !d[0].alive && !d[2].alive && !d[0].enabled && d[1].alive;
    std::printf(ok ? "0065b142_TEST PASS\n" : "0065b142_TEST FAIL\n");
    return ok ? 0 : 1;
}
