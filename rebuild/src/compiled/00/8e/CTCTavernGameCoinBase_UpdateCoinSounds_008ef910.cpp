// CTCTavernGameCoinBase::UpdateCoinSounds(float elapsed) -- forwards the frame argument to Coin::UpdateSound for every coin record in the +0xFC vector
// Retail: Fable.exe @ 0x008EF910, 50 bytes (`ret 4`).
// Callees: Coin::UpdateSound 0x008EF400 (thiscall on each 0x3C-byte coin record, one stack argument).
// Shape: plain `for` over [first, last) of the coin vector: the argument is loaded once into ebx
//        inside the non-empty path, `last` is re-read after every call (the callee may touch it).
struct CTavernCoin {                    // 0x3C bytes
    unsigned char _pad[0x3c];
    void UpdateSound(float elapsed);    // 0x008EF400
};

struct CTCTavernGameCoinBase {
    unsigned char _pad[0xfc];
    CTavernCoin* m_coinsFirst;          // +0xfc vector<Coin> begin
    CTavernCoin* m_coinsLast;           // +0x100 end
    void UpdateCoinSounds(float elapsed);
};

void CTCTavernGameCoinBase::UpdateCoinSounds(float elapsed)
{
    for (CTavernCoin* coin = m_coinsFirst; coin != m_coinsLast; ++coin)
        coin->UpdateSound(elapsed);
}