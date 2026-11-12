#include <cstdio>
// model of 0x00C0AE70 / 0x00C0AF00: bit `index` (< count) is ORed into `changed` and set/cleared in `state`
static void SetMaskBit(unsigned long index, bool set, unsigned short* changed, unsigned short* state, unsigned long count)
{
    if (index >= count) return;
    unsigned short bit = (unsigned short)(1u << index);
    if (changed) *changed |= bit;
    if (state) { if (set) *state |= bit; else *state &= (unsigned short)~bit; }
}
static void SetMaskBit8(unsigned long index, bool set, unsigned short* changed, unsigned short* state) { SetMaskBit(index, set, changed, state, 8); }
struct CBitMaskPair_00c0a8c0 {
    unsigned short m_stateMask; unsigned short m_changedMask;
    void SetFlag(unsigned long index, bool set) { SetMaskBit8(index, set, &m_changedMask, &m_stateMask); }
};
int main()
{
    CBitMaskPair_00c0a8c0 m; m.m_stateMask = 0; m.m_changedMask = 0;
    m.SetFlag(3, true);
    if (m.m_stateMask != 8 || m.m_changedMask != 8) { std::printf("00c0a8c0_TEST FAIL\n"); return 1; }
    m.SetFlag(0, true); m.SetFlag(3, false);
    if (m.m_stateMask != 1 || m.m_changedMask != 9) { std::printf("00c0a8c0_TEST FAIL\n"); return 1; }
    m.SetFlag(9, true);
    if (m.m_stateMask != 1 || m.m_changedMask != 9) { std::printf("00c0a8c0_TEST FAIL\n"); return 1; }
    std::printf("00c0a8c0_TEST PASS\n");
    return 0;
}