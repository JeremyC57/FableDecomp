#include <cstdio>
struct CTCRewardsHandled { bool m_handled; };
struct CThing {
    unsigned char m_type; bool m_hasA0; CTCRewardsHandled* m_pA0;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (type == 0xa0 && m_hasA0) { *out = (T*)m_pA0; return true; }
        return false;
    }
};
static int g_drops = 0;
struct CTCContainerRewardHero {
    CThing* ParentThing; bool Enabled;
    void CreateItemPickupContainer() { ++g_drops; }
    void OnDie()
    {
        CThing* owner = ParentThing;
        CTCRewardsHandled* handled;
        if (owner->GetInterface(0xa0, &handled) && handled->m_handled)
            return;
        if (Enabled && owner->m_type != 2 && owner->m_type != 0xd)
            CreateItemPickupContainer();
    }
};
int main()
{
    CTCRewardsHandled h; h.m_handled = false;
    CThing barrel; barrel.m_type = 5; barrel.m_hasA0 = false; barrel.m_pA0 = &h;
    CTCContainerRewardHero c; c.ParentThing = &barrel; c.Enabled = true;
    c.OnDie();                                        // drops
    barrel.m_hasA0 = true; c.OnDie();                 // component present, not handled: drops
    h.m_handled = true; c.OnDie();                    // handled: no drop
    h.m_handled = false; barrel.m_type = 2; c.OnDie(); // creature: no drop
    barrel.m_type = 0xd; c.OnDie();                   // type 13: no drop
    barrel.m_type = 5; c.Enabled = false; c.OnDie();  // disabled: no drop
    if (g_drops != 2) { std::printf("0074f080_TEST FAIL\n"); return 1; }
    std::printf("0074f080_TEST PASS\n");
    return 0;
}