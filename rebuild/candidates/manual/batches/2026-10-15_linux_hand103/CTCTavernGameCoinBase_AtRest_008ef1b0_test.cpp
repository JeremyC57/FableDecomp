#include <cstdio>
// Behaviour test for CTCTavernGameCoinBase::AtRest (0x008EF1B0).
struct C2DVector { float x; float y; float GetLengthSquared() const { return x * x + y * y; } };
struct CTavernCoin { unsigned char _pad[0xc]; C2DVector m_velocity; unsigned char _pad_0x14[0x28]; };
struct CTCTavernGameCoinBase {
    unsigned char _pad[0xfc];
    CTavernCoin* m_coinsFirst; CTavernCoin* m_coinsLast;
    bool AtRest(float speedThreshold);
};
bool CTCTavernGameCoinBase::AtRest(float speedThreshold)
{
    for (CTavernCoin* coin = m_coinsFirst; coin != m_coinsLast; ++coin) {
        if (coin->m_velocity.GetLengthSquared() > speedThreshold * speedThreshold)
            return false;
    }
    return true;
}
int main()
{
    static CTCTavernGameCoinBase g;
    CTavernCoin coins[3];
    coins[0].m_velocity.x = 0.1f; coins[0].m_velocity.y = 0.0f;
    coins[1].m_velocity.x = 0.0f; coins[1].m_velocity.y = -0.2f;
    coins[2].m_velocity.x = 3.0f; coins[2].m_velocity.y = 4.0f;   // speed 5
    g.m_coinsFirst = coins; g.m_coinsLast = coins;
    if (!g.AtRest(0.5f)) { std::printf("008ef1b0_TEST FAIL\n"); return 1; }        // empty
    g.m_coinsLast = coins + 2;
    if (!g.AtRest(0.5f) || g.AtRest(0.15f)) { std::printf("008ef1b0_TEST FAIL\n"); return 1; }
    g.m_coinsLast = coins + 3;
    if (g.AtRest(4.9f) || !g.AtRest(5.0f)) { std::printf("008ef1b0_TEST FAIL\n"); return 1; }
    std::printf("008ef1b0_TEST PASS\n");
    return 0;
}
