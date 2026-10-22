// Behaviour test for CPlayerGui::DrawInventory @ 0x00438CB0: a live controlled hero asks each inventory component it
// carries (interface bitset test, then the component lookup) to draw, in the fixed retail order.
#include <cstdio>
#include <vector>
#include <map>
struct Component {
    int Type;
    std::vector<int>* Log;
    void DrawInventory(void* world) { (void)world; Log->push_back(Type); }
};
struct Thing {
    unsigned int PresentBits[9];
    std::map<int, Component*> Components;
    unsigned char StatusFlags;
    Component* GetInterface(int t) { return Components[t]; }
};
static void DrawInventory(Thing* hero, void* world)
{
    if (!hero || (hero->StatusFlags & 1))
        return;
    static const int order[14] = { 0x11, 0x12, 0x13, 0x6f, 0x16, 0x19, 0x17, 0x14, 0x18, 0x15, 0x1b, 0xc6, 0xfc, 0x10e };
    for (int i = 0; i < 14; ++i) {
        int t = order[i];
        if (hero->PresentBits[t >> 5] & (1U << (t & 31)))
            hero->GetInterface(t)->DrawInventory(world);
    }
}
int main()
{
    std::vector<int> log;
    Thing hero;
    for (int i = 0; i < 9; ++i) hero.PresentBits[i] = 0;
    hero.StatusFlags = 0;
    static Component c[4] = { { 0x13, 0 }, { 0x6f, 0 }, { 0x15, 0 }, { 0x10e, 0 } };
    for (int i = 0; i < 4; ++i) {
        c[i].Log = &log;
        hero.PresentBits[c[i].Type >> 5] |= 1U << (c[i].Type & 31);
        hero.Components[c[i].Type] = &c[i];
    }
    DrawInventory(&hero, 0);
    bool ok = log.size() == 4 && log[0] == 0x13 && log[1] == 0x6f && log[2] == 0x15 && log[3] == 0x10e;
    hero.StatusFlags = 1;
    DrawInventory(&hero, 0);
    DrawInventory(0, 0);
    ok = ok && log.size() == 4;
    if (!ok) { std::printf("00438cb0_TEST FAIL\n"); return 1; }
    std::printf("00438cb0_TEST PASS\n");
    return 0;
}
