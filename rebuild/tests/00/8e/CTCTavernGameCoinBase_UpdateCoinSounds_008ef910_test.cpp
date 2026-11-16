#include <cstdio>
// Behaviour test for CTCTavernGameCoinBase::UpdateCoinSounds (0x008EF910).
struct CTavernCoin {
    bool lastOn; int updates; unsigned char _pad[0x34];
    void UpdateSound(bool on) { lastOn = on; ++updates; }   // NF37: Coin::UpdateSound(bool) (Ego_r.pdb)
};
struct CTCTavernGameCoinBase {
    unsigned char _pad[0xfc];
    CTavernCoin* m_coinsFirst; CTavernCoin* m_coinsLast;
    void UpdateCoinSounds(bool on);   // NF37: Ego_r.pdb / FableWin.pdb ?UpdateCoinSounds@CTCTavernGameCoinBase@@QAEX_N@Z
};
void CTCTavernGameCoinBase::UpdateCoinSounds(bool on)
{
    for (CTavernCoin* coin = m_coinsFirst; coin != m_coinsLast; ++coin)
        coin->UpdateSound(on);
}
int main()
{
    static CTCTavernGameCoinBase g;
    CTavernCoin coins[3];
    for (int i = 0; i < 3; ++i) { coins[i].updates = 0; coins[i].lastOn = false; }
    g.m_coinsFirst = coins; g.m_coinsLast = coins + 2;
    g.UpdateCoinSounds(true);
    if (coins[0].updates != 1 || coins[1].updates != 1 || coins[2].updates != 0 || coins[1].lastOn != true)
    { std::printf("008ef910_TEST FAIL\n"); return 1; }
    g.m_coinsLast = coins;
    g.UpdateCoinSounds(false);
    if (coins[0].updates != 1) { std::printf("008ef910_TEST FAIL\n"); return 1; }
    std::printf("008ef910_TEST PASS\n");
    return 0;
}