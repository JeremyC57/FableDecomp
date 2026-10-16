// behaviour test for 0x007C8760 CTCFireballSpell::~CTCFireballSpell
#include <cstdio>
#include <cstdlib>
static int g_defDestroyed = 0;
struct Thing { bool dead; int kills; void Kill(bool) { ++kills; dead = true; } };
struct Ptr { Thing* item; Thing* GetPItem() const { return item; } };
struct Def { long RefCount; void Release() { if (--RefCount == 0) ++g_defDestroyed; } };
struct DefPtr { Def* Object; ~DefPtr() { if (Object) { Object->Release(); Object = 0; } } };
struct Vec { float* start; ~Vec() { if (start) std::free(start); } };
struct Spell {
    Vec timings; DefPtr PDef; Ptr effect;
    ~Spell() { if (effect.GetPItem() && !effect.GetPItem()->dead) effect.GetPItem()->Kill(true); }
};
int main()
{
    Thing t = { false, 0 };
    Def d = { 1 };
    {
        Spell s; s.timings.start = (float*)std::malloc(16); s.PDef.Object = &d; s.effect.item = &t;
    }
    bool ok = t.kills == 1 && g_defDestroyed == 1;
    {
        Spell s; s.timings.start = 0; s.PDef.Object = 0; s.effect.item = &t;   // already dead: no second kill
    }
    ok = ok && t.kills == 1;
    if (!ok) { std::printf("007c8760_TEST FAIL\n"); return 1; }
    std::printf("007c8760_TEST PASS\n");
    return 0;
}
