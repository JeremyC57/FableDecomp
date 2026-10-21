// Behaviour test for 008981c0 CGameScriptInterface::AddTattooToHero: spawn point = position + 2 x facing, morph gets the tattoo, object killed.
#include <cstdio>
struct V { float x, y, z; };
struct Hero { bool dead; bool hasMorph; V pos; V facing; int tattoos; int uploads; };
struct Obj { V pos; bool killed; };
static bool AddTattoo(long index, Hero* hero, Obj& obj)
{
    if (index <= 0) return false;
    if (!hero || hero->dead) return false;
    V off = { hero->facing.x * 2.0f, hero->facing.y * 2.0f, hero->facing.z * 2.0f };
    V p = { hero->pos.x + off.x, hero->pos.y + off.y, hero->pos.z + off.z };
    obj.pos = p; obj.killed = false;
    if (hero->hasMorph) { ++hero->tattoos; if (!hero->dead && hero->hasMorph) ++hero->uploads; }
    obj.killed = true;
    return true;
}
int main()
{
    Hero h = { false, true, { 1, 2, 3 }, { 0.5f, -1, 0 }, 0, 0 };
    Obj o;
    bool r = AddTattoo(4, &h, o);
    Obj o2 = { { 0, 0, 0 }, false };
    bool r2 = AddTattoo(0, &h, o2);
    if (!r || r2 || o.pos.x != 2 || o.pos.y != 0 || o.pos.z != 3 || h.tattoos != 1 || h.uploads != 1 || !o.killed || o2.killed) {
        std::printf("008981c0_TEST FAIL\n"); return 1; }
    std::printf("008981c0_TEST PASS\n");
    return 0;
}
