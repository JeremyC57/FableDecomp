#include <cstdio>
// Behaviour test for CTCTavernGameCoinBase::UpdateCoinSounds (0x008EF910).
struct CTavernCoin {
    float lastElapsed; int updates; unsigned char _pad[0x34];
    void UpdateSound(float elapsed) { lastElapsed = elapsed; ++updates; }
};
struct CTCTavernGameCoinBase {
    unsigned char _pad[0xfc];
    CTavernCoin* m_coinsFirst; CTavernCoin* m_coinsLast;
    void UpdateCoinSounds(float elapsed);
};
void CTCTavernGameCoinBase::UpdateCoinSounds(float elapsed)
{
    for (CTavernCoin* coin = m_coinsFirst; coin != m_coinsLast; ++coin)
        coin->UpdateSound(elapsed);
}
int main()
{
    static CTCTavernGameCoinBase g;
    CTavernCoin coins[3];
    for (int i = 0; i < 3; ++i) { coins[i].updates = 0; coins[i].lastElapsed = 0.0f; }
    g.m_coinsFirst = coins; g.m_coinsLast = coins + 2;
    g.UpdateCoinSounds(0.25f);
    if (coins[0].updates != 1 || coins[1].updates != 1 || coins[2].updates != 0 || coins[1].lastElapsed != 0.25f)
    { std::printf("008ef910_TEST FAIL\n"); return 1; }
    g.m_coinsLast = coins;
    g.UpdateCoinSounds(1.0f);
    if (coins[0].updates != 1) { std::printf("008ef910_TEST FAIL\n"); return 1; }
    std::printf("008ef910_TEST PASS\n");
    return 0;
}
