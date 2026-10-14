// CChannelState_00c592d0::Clear() -- zero the 0x160-byte sample block at +4 (`rep stosd`, 88 words) and the two counters at +0x164 and +0x174
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C592D0, length 28 bytes (the oracle row carries 4 bytes of int3 padding).
// Callees: none (memset is the intrinsic `rep stosd`). Shape: `this` moves to edx so ecx can be the stosd count, `lea edi,[edx+4]` is the
// block's address, the zero register then clears the two words; no `this` return, so a void member rather than a constructor. The
// manifest label "CEntry::CEntry" is propagated (libpk audio module); the class is named by address.
#include <string.h>

struct CChannelState_00c592d0 {
    long          m_0;                             // +0x000 (not touched)
    unsigned char m_block[0x160];                  // +0x004 the sample/state block
    long          m_164;                           // +0x164
    long          m_168, m_16c, m_170;             // +0x168..+0x170 (not touched)
    long          m_174;                           // +0x174
    void Clear();
};

void CChannelState_00c592d0::Clear()
{
    memset(m_block, 0, sizeof(m_block));
    m_164 = 0;
    m_174 = 0;
}
