// Behaviour test for 0x008c4040 (wasp-queen attack Initialise): the animation name is set, the base runs, the old type-200
// component is removed and a fresh CTCHitOnCollision is added with its object-hit byte armed (a refused add is tolerated).
#include <cstdio>
#include <cstring>
static int g_base = 0, g_removed = -1; static char g_anim[64]; static char g_added[64];
struct CHit { unsigned char m_hitsObjects; };
static CHit g_hit;
class CThing { public:
    bool m_refuse;
    void RemoveTC(int type) { g_removed = type; }
    CHit* AddTC(const char* name, bool, void*) { std::strcpy(g_added, name); return m_refuse ? 0 : &g_hit; }
};
struct CAction {
    CThing* m_creature;
    CThing* GetPItem() { return m_creature; }
    void SetCombatAnimation(const char* n) { std::strcpy(g_anim, n); }
    void BaseInitialise() { ++g_base; }
    void Initialise()
    {
        SetCombatAnimation("STRIKE_MEDIUM_FRONT");
        BaseInitialise();
        GetPItem()->RemoveTC(200);
        CHit* pHit = GetPItem()->AddTC("CTCHitOnCollision", false, 0);
        if (pHit)
            pHit->m_hitsObjects = 1;
    }
};
int main()
{
    CThing t; t.m_refuse = true; g_hit.m_hitsObjects = 0;
    CAction a; a.m_creature = &t;
    a.Initialise();
    if (g_base != 1 || g_removed != 200 || std::strcmp(g_anim, "STRIKE_MEDIUM_FRONT") || std::strcmp(g_added, "CTCHitOnCollision") || g_hit.m_hitsObjects)
        { std::printf("008c4040_TEST FAIL\n"); return 1; }
    t.m_refuse = false;
    a.Initialise();
    if (g_base != 2 || g_hit.m_hitsObjects != 1) { std::printf("008c4040_TEST FAIL\n"); return 1; }
    std::printf("008c4040_TEST PASS\n");
    return 0;
}