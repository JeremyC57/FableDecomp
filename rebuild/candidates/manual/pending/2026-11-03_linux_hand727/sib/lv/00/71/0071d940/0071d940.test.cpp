// behaviour test for 0x0071D940 CTCHeroMorph::AddTattoo(CDefPointer<const CThingBaseDef>&)
#include <cstdio>
#include <vector>
struct Tattoo { bool removesAll; long covers; long specific; };
struct Morph { std::vector<const Tattoo*> worn; bool changed; long frame; };
static bool Clash(const Tattoo* w, const Tattoo* t) {
    long overlap = w->covers & t->covers;
    if (overlap == 0) return false;
    return overlap != 8 || (w->specific & t->specific) != 0 || w->specific == 0 || t->specific == 0;
}
static void AddTattoo(Morph& m, const Tattoo* t, long frame) {
    if (!t) return;
    if (t->removesAll) m.worn.clear();
    else {
        for (size_t i = 0; i < m.worn.size(); ) { if (m.worn[i] && Clash(m.worn[i], t)) m.worn.erase(m.worn.begin() + i); else ++i; }
        m.worn.push_back(t);
    }
    m.changed = true; m.frame = frame;
}
int main() {
    Tattoo arm = {false, 1, 0}, face = {false, 8, 2}, face2 = {false, 8, 4}, face3 = {false, 8, 2}, wipe = {true, 0, 0};
    Morph m = {std::vector<const Tattoo*>(), false, 0};
    AddTattoo(m, &arm, 3); AddTattoo(m, &face, 4); AddTattoo(m, &face2, 5);
    if (m.worn.size() != 3 || !m.changed || m.frame != 5) { std::printf("0071d940_TEST FAIL\n"); return 1; }
    AddTattoo(m, &face3, 6);
    if (m.worn.size() != 3 || m.worn[2] != &face3 || m.worn[1] != &face2) { std::printf("0071d940_TEST FAIL\n"); return 1; }
    AddTattoo(m, &wipe, 7);
    if (!m.worn.empty() || m.frame != 7) { std::printf("0071d940_TEST FAIL\n"); return 1; }
    m.changed = false; AddTattoo(m, 0, 8);
    if (m.changed) { std::printf("0071d940_TEST FAIL\n"); return 1; }
    std::printf("0071d940_TEST PASS\n");
    return 0;
}