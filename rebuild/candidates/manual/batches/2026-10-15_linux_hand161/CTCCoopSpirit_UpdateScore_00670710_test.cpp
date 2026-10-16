// behaviour test for 0x00670710 CTCCoopSpirit::UpdateScore
#include <cstdio>
struct Score { long m_counter; };
struct CThing {
    unsigned int bits[9]; Score* comp;
    template<class T> bool GetInterface(T** out, long type) { if (bits[type >> 5] & (1u << (type & 31))) { *out = comp; return true; } return false; }
};
struct Player { CThing* hero; CThing* GetHeroThing() { return hero; } };
struct PlayerManager { Player players[4]; long main; long GetMainPlayerNumber() { return main; } Player* GetPlayer(long n) { return &players[n]; } };
struct Spirit {
    long Score_; PlayerManager* pm; int swaps;
    void SwapToHero() { ++swaps; }
    void UpdateScore()
    {
        if (Score_ >= 20) {
            for (long p = 0; p < 4; ++p)
                if (p != pm->GetMainPlayerNumber()) {
                    CThing* hero = pm->GetPlayer(p)->GetHeroThing();
                    Score* s;
                    if (hero && hero->GetInterface(&s, 0xc7)) s->m_counter = 0;
                }
            SwapToHero();
        }
    }
};
int main()
{
    Score sc[4] = { { 5 }, { 6 }, { 7 }, { 8 } };
    CThing h[4];
    PlayerManager pm; pm.main = 1;
    for (int i = 0; i < 4; ++i) { for (int j = 0; j < 9; ++j) h[i].bits[j] = 0; h[i].bits[6] = 0x80; h[i].comp = &sc[i]; pm.players[i].hero = &h[i]; }
    h[3].bits[6] = 0; pm.players[2].hero = 0;
    Spirit s; s.Score_ = 19; s.pm = &pm; s.swaps = 0;
    s.UpdateScore();
    bool ok = s.swaps == 0 && sc[0].m_counter == 5;
    s.Score_ = 20; s.UpdateScore();
    ok = ok && s.swaps == 1 && sc[0].m_counter == 0 && sc[1].m_counter == 6 && sc[2].m_counter == 7 && sc[3].m_counter == 8;
    if (!ok) { std::printf("00670710_TEST FAIL\n"); return 1; }
    std::printf("00670710_TEST PASS\n");
    return 0;
}
