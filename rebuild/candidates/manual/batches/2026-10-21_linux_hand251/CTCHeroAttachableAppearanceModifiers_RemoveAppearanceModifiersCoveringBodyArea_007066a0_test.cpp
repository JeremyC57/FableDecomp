#include <cstdio>
#include <vector>
#include <list>
// behaviour test: RemoveAppearanceModifiersCoveringBodyArea -- collects the type's modifiers whose def covers any requested area
// (for type 2 a modifier sharing only the 0x100 area with a 0x100 request is kept), removes them in order, stamps the frame
struct Mods {
    std::vector<long> Worn[3]; long Covers[16]; bool HasDef[16]; long Frame; std::vector<long> Removed;
    void Remove(long defIndex) { Removed.push_back(defIndex); }
    void RemoveCovering(long area, int type, long now)
    {
        std::list<long> toRemove;
        for (unsigned i = 0; i < Worn[type].size(); ++i) {
            long d = Worn[type][i];
            if (!HasDef[d]) continue;
            long c = Covers[d];
            if (c & area) {
                bool remove = true;
                if (type == 2 && (area & 0x100) && (c & 0x100)) remove = (c & area & ~0x100) != 0;
                if (remove) toRemove.push_back(d);
            }
        }
        for (std::list<long>::iterator it = toRemove.begin(); it != toRemove.end(); ++it) Remove(*it);
        Frame = now;
    }
};
int main()
{
    Mods m;
    for (int i = 0; i < 16; ++i) { m.Covers[i] = 0; m.HasDef[i] = true; }
    m.Covers[1] = 0x1; m.Covers[2] = 0x2; m.Covers[3] = 0x101; m.Covers[4] = 0x100; m.HasDef[5] = false; m.Covers[5] = 0x1;
    m.Worn[0].push_back(1); m.Worn[0].push_back(2); m.Worn[0].push_back(5);
    m.RemoveCovering(0x1, 0, 7);
    bool ok = m.Removed.size() == 1 && m.Removed[0] == 1 && m.Frame == 7;
    m.Removed.clear();
    m.Worn[2].push_back(3); m.Worn[2].push_back(4); m.Worn[2].push_back(2);
    m.RemoveCovering(0x101, 2, 9);                    // 3 shares 0x1 too: removed; 4 shares only 0x100: kept; 2 no overlap
    ok = ok && m.Removed.size() == 1 && m.Removed[0] == 3 && m.Frame == 9;
    m.Removed.clear();
    m.RemoveCovering(0x100, 1, 11);                   // empty type: just the frame
    ok = ok && m.Removed.empty() && m.Frame == 11;
    if (!ok) { std::printf("007066a0_TEST FAIL\n"); return 1; }
    std::printf("007066a0_TEST PASS\n");
    return 0;
}
