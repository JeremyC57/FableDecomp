// Behaviour test for 0x00AD9770 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
#include <algorithm>
typedef std::vector<int> Block;
static bool ProperSubset(const Block& a, const Block& b) {
    if (a.size() >= b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) if (std::find(b.begin(), b.end(), a[i]) == b.end()) return false;
    return true;
}
static void RemoveAllSubsets(std::vector<Block>& blocks) {
    std::vector<Block> kept;
    for (size_t i = 0; i < blocks.size(); ++i) { bool sub = false; for (size_t j = 0; j < blocks.size(); ++j) if (ProperSubset(blocks[i], blocks[j])) { sub = true; break; } if (!sub) kept.push_back(blocks[i]); }
    blocks = kept;
}
int main() {
    std::vector<Block> b(3); b[0].push_back(1); b[1].push_back(1); b[1].push_back(2); b[2].push_back(3);
    RemoveAllSubsets(b); bool ok = b.size() == 2 && b[0].size() == 2 && b[1][0] == 3;
    std::printf(ok ? "00ad9770_TEST PASS\n" : "00ad9770_TEST FAIL\n");
    return ok ? 0 : 1;
}
