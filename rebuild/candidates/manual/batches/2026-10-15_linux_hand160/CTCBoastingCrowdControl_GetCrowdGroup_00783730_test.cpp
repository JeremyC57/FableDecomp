// Behaviour test for CTCBoastingCrowdControl::GetCrowdGroup (0x00783730): false for a null out pointer, an unknown hero, a dead or
// missing crowd thing or one without the boasting-crowd group; otherwise the component is returned.
#include <cstdio>
struct Group { int id; };
struct Thing {
    unsigned int uid; unsigned int bits[9]; unsigned char Flags91; Group* group;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    bool GetInterface(Group** o, int t) { if (bits[t >> 5] & (1u << (t & 31))) { *o = group; return true; } return false; }
};
struct Entry { unsigned int key; Thing* crowd; };
struct Control {
    Entry e[2]; int n;
    const Entry* find(unsigned int k) const { for (int i = 0; i < n; ++i) if (e[i].key == k) return &e[i]; return 0; }
    bool GetCrowdGroup(Thing& hero, Group** out) const
    {
        if (out) {
            const Entry* it = find(hero.uid);
            if (it) {
                Thing* crowd = it->crowd;
                if (crowd && !crowd->IsDead() && crowd->GetInterface(out, 0x74)) return true;
            }
        }
        return false;
    }
};
int main()
{
    Group g; Thing crowd; for (int i = 0; i < 9; ++i) crowd.bits[i] = 0; crowd.bits[3] = 1u << 20; crowd.Flags91 = 0; crowd.group = &g;
    Thing hero; hero.uid = 7; Control c; c.n = 1; c.e[0].key = 7; c.e[0].crowd = &crowd;
    Group* out = 0;
    if (!c.GetCrowdGroup(hero, &out) || out != &g) { std::printf("00783730_TEST FAIL\n"); return 1; }
    if (c.GetCrowdGroup(hero, 0)) { std::printf("00783730_TEST FAIL\n"); return 1; }
    crowd.Flags91 = 1; if (c.GetCrowdGroup(hero, &out)) { std::printf("00783730_TEST FAIL\n"); return 1; }
    hero.uid = 8; crowd.Flags91 = 0; if (c.GetCrowdGroup(hero, &out)) { std::printf("00783730_TEST FAIL\n"); return 1; }
    std::printf("00783730_TEST PASS\n");
    return 0;
}
