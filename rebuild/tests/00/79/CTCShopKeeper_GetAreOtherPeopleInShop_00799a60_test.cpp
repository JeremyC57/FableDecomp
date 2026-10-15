#include <cstdio>
static long g_time = 0;
static int g_calcs = 0;
struct CWorld { static long GetWorldFrame() { return g_time; } };
struct CTCShopKeeper {
    bool m_areOtherPeopleInShop; long m_lastCheckTime; bool m_next;
    bool CalcAreOtherPeopleInShop() { ++g_calcs; return m_next; }
    bool GetAreOtherPeopleInShop(bool force)
    {
        if (force || CWorld::GetWorldFrame() - m_lastCheckTime > 15) {
            m_lastCheckTime = CWorld::GetWorldFrame();
            m_areOtherPeopleInShop = CalcAreOtherPeopleInShop();
        }
        return m_areOtherPeopleInShop;
    }
};
int main()
{
    CTCShopKeeper k; k.m_areOtherPeopleInShop = false; k.m_lastCheckTime = 0; k.m_next = true;
    g_time = 10;
    if (k.GetAreOtherPeopleInShop(false) != false || g_calcs != 0) { std::printf("00799a60_TEST FAIL\n"); return 1; }
    g_time = 16;
    if (k.GetAreOtherPeopleInShop(false) != true || g_calcs != 1 || k.m_lastCheckTime != 16) { std::printf("00799a60_TEST FAIL\n"); return 1; }
    k.m_next = false; g_time = 17;
    if (k.GetAreOtherPeopleInShop(true) != false || g_calcs != 2) { std::printf("00799a60_TEST FAIL\n"); return 1; }
    std::printf("00799a60_TEST PASS\n");
    return 0;
}