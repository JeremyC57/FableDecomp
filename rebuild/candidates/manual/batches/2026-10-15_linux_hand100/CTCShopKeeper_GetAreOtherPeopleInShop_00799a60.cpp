// CTCShopKeeper::GetAreOtherPeopleInShop(bool) -- cached "other people in the shop" flag (+0x2c), recomputed when forced or when more than 15 world frames have passed since the last check (+0x30)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799A60, length 49 bytes (+15 int3 padding).
// Callees: the static CWorld::GetWorldFrame @ 0x0049D870 (as landed in 0x008EC7E0; the manifest label
// "GFGetPreMainMemoryUsed" is propagated), called twice; CTCShopKeeper::CalcAreOtherPeopleInShop @ 0x00799140.
// Shape: `force || now - last > 15` (signed `jle`), the time re-read for the store, the flag returned from memory.
struct CWorld {
    static long GetWorldFrame();             // 0x0049D870
};

struct CTCShopKeeper {
    unsigned char _pad[0x2c];
    bool          m_areOtherPeopleInShop;    // +0x2c
    unsigned char _pad2d[3];
    long          m_lastCheckTime;           // +0x30
    bool CalcAreOtherPeopleInShop();         // 0x00799140
    bool GetAreOtherPeopleInShop(bool force);
};

bool CTCShopKeeper::GetAreOtherPeopleInShop(bool force)
{
    if (force || CWorld::GetWorldFrame() - m_lastCheckTime > 15) {
        m_lastCheckTime = CWorld::GetWorldFrame();
        m_areOtherPeopleInShop = CalcAreOtherPeopleInShop();
    }
    return m_areOtherPeopleInShop;
}
