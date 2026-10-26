// Behaviour test for 0x0094EF90: FollowPlayer::IsRunnable remembers a live hero.
#include <cstdio>
class CThing { public: unsigned char m_flags91; };
struct CGroup { CThing* hero; CThing* PThingToFollow;
    bool IsRunnable()
    {
        CThing* h = hero;
        if (h != 0 && !(h->m_flags91 & 1)) { PThingToFollow = h; return true; }
        return false;
    }
};
int main()
{
    CThing h = { 0 };
    CGroup g = { &h, 0 };
    int bad = 0;
    if (!g.IsRunnable() || g.PThingToFollow != &h) bad = 1;
    h.m_flags91 = 1; g.PThingToFollow = 0; if (g.IsRunnable() || g.PThingToFollow) bad = 2;
    g.hero = 0; if (g.IsRunnable()) bad = 3;
    if (bad) { std::printf("0094ef90_TEST FAIL\n"); return 1; }
    std::printf("0094ef90_TEST PASS\n");
    return 0;
}