#include <cstdio>
// Behaviour model of the entity-script constructor @ 0x00ee02c0: Game from the parent's interface, Me copied, parent and
// master data stored, the two extra script things empty.
struct Thing { void* data; void* info; Thing() : data(0), info(0) { } };
struct Owner { void* iface; };
struct Script {
    void* game; Thing me; Owner* parent; void* master; Thing a, b;
    Script(Owner* p, void* m, const Thing& t) : game(p->iface), me(t), parent(p) { master = m; }
};
int main()
{
    int iface = 0, master = 0, data = 0;
    Owner o = { &iface };
    Thing t; t.data = &data;
    Script s(&o, &master, t);
    if (s.game != &iface || s.me.data != &data || s.parent != &o || s.master != &master || s.a.data || s.b.info) { std::printf("00ee02c0_TEST FAIL\n"); return 1; }
    std::printf("00ee02c0_TEST PASS\n");
    return 0;
}
