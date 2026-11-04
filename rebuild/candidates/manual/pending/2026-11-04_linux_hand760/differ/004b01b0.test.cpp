// Behaviour test for 0x004B01B0 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
struct Quest { std::vector<int> Regions; bool Contains(int r) const { for (size_t i = 0; i < Regions.size(); ++i) if (Regions[i] == r) return true; return false; } };
static bool Valid(const std::vector<const Quest*>& active, int region) {
    bool questHasRegions = false;
    for (size_t i = 0; i < active.size(); ++i) if (active[i]) { if (active[i]->Contains(region)) return true; if (!active[i]->Regions.empty()) questHasRegions = true; }
    return !questHasRegions;
}
int main() {
    Quest a, b; a.Regions.push_back(3); std::vector<const Quest*> q; q.push_back(&b);
    bool ok = Valid(q, 5);
    q.push_back(&a); ok = ok && Valid(q, 3) && !Valid(q, 5);
    q.push_back(0); ok = ok && !Valid(q, 4);
    std::printf(ok ? "004b01b0_TEST PASS\n" : "004b01b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
