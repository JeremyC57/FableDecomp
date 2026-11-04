// Behaviour test for 0x00ACD960 QuadricOptimiserInternals::COptimisedPrimitive::SimplifyByVertexPairs: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <map>
struct C { bool valid; float cost; bool fold; int performed; };
static void Simplify(std::multimap<float, C*>& m, float maxCost, float penalty)
{
    while (!m.empty()) {
        C* c = m.begin()->second;
        if (!c->valid) { m.erase(m.begin()); continue; }
        if (c->cost > maxCost) return;
        if (c->fold) { c->cost += penalty; c->fold = false; m.erase(m.begin()); m.insert(std::make_pair(c->cost, c)); }
        else { c->performed++; c->valid = false; m.erase(m.begin()); }
    }
}
int main() {
    C a = { true, 1.0f, false, 0 }, b = { true, 2.0f, true, 0 }, c = { false, 0.5f, false, 0 }, d = { true, 9.0f, false, 0 };
    std::multimap<float, C*> m; m.insert(std::make_pair(1.0f, &a)); m.insert(std::make_pair(2.0f, &b)); m.insert(std::make_pair(0.5f, &c)); m.insert(std::make_pair(9.0f, &d));
    Simplify(m, 5.0f, 1.0f);
    int ok = a.performed == 1 && b.performed == 1 && b.cost == 3.0f && d.performed == 0 && m.size() == 1;
    std::printf(ok ? "00acd960_TEST PASS\n" : "00acd960_TEST FAIL\n");
    return ok ? 0 : 1;
}
