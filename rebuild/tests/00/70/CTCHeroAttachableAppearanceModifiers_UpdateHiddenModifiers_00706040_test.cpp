// 00706040 behaviour test: CTCHeroAttachableAppearanceModifiers::UpdateHiddenModifiers (final.cpp body over mock defs)
#include <stdio.h>
#include <vector>
#include <map>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct CModifierEntry { long DefIndex; bool Visible; };
static std::map<long, unsigned long> g_masks;   // def index -> hide mask (absent: no def; 0x80000000 bit: def without modifier sub-def)
static bool GetDef(long index) { return index > 0 && g_masks.count(index) != 0; }
static bool GetSubDef(long index, unsigned long* mask) { unsigned long m = g_masks[index]; if (m & 0x80000000u) return false; *mask = m; return true; }
struct CTCHeroAttachableAppearanceModifiers {
    std::vector< std::vector<CModifierEntry> > AppearanceModifiers;
    void UpdateHiddenModifiers();
};
void CTCHeroAttachableAppearanceModifiers::UpdateHiddenModifiers()
{
    for (int layer = 0; layer < 3; ++layer) {
        std::vector<CModifierEntry>& v = AppearanceModifiers[layer];
        for (size_t e = 0; e < v.size(); ++e)
            v[e].Visible = true;
    }
    for (int i = 2; i > 0; --i) {
        std::vector<CModifierEntry>& upper = AppearanceModifiers[i];
        for (size_t e = 0; e < upper.size(); ++e) {
            if (!upper[e].Visible)
                continue;
            if (!GetDef(upper[e].DefIndex))
                continue;
            unsigned long mask;
            if (GetSubDef(upper[e].DefIndex, &mask)) {
                for (int j = i - 1; j >= 0; --j) {
                    std::vector<CModifierEntry>& lower = AppearanceModifiers[j];
                    for (size_t l = 0; l < lower.size(); ++l) {
                        if (!GetDef(lower[l].DefIndex))
                            continue;
                        unsigned long lmask;
                        if (GetSubDef(lower[l].DefIndex, &lmask) && (lmask & mask))
                            lower[l].Visible = false;
                    }
                }
            }
        }
    }
}
int main()
{
    g_masks[1] = 0x1; g_masks[2] = 0x2; g_masks[3] = 0x3; g_masks[4] = 0x80000001u; g_masks[5] = 0x4; g_masks[6] = 0x2;
    CTCHeroAttachableAppearanceModifiers m; m.AppearanceModifiers.resize(3);
    CModifierEntry a = { 1, false }, b = { 2, false }, c = { 5, false }, d = { 9, false }, e = { 6, false }, f = { 3, false }, g = { 4, false };
    m.AppearanceModifiers[0].push_back(a); m.AppearanceModifiers[0].push_back(b); m.AppearanceModifiers[0].push_back(c); m.AppearanceModifiers[0].push_back(d);
    m.AppearanceModifiers[1].push_back(e); m.AppearanceModifiers[1].push_back(g);
    m.AppearanceModifiers[2].push_back(f);
    m.UpdateHiddenModifiers();
    // layer 2 def 3 (mask 3) hides layer-1 def 6 (mask 2) and layer-0 defs 1, 2; layer-1 def 4 has no modifier sub-def
    CHECK(!m.AppearanceModifiers[1][0].Visible && m.AppearanceModifiers[1][1].Visible);
    CHECK(!m.AppearanceModifiers[0][0].Visible && !m.AppearanceModifiers[0][1].Visible);
    CHECK(m.AppearanceModifiers[0][2].Visible && m.AppearanceModifiers[0][3].Visible && m.AppearanceModifiers[2][0].Visible);
    if (!g_fail) printf("00706040_TEST PASS\n");
    return g_fail;
}