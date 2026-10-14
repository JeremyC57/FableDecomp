// CScreenEffectOwner_00c55c80::Reset() -- reset the priority stack base (0x00C55B70 with 1), then clear the nine dwords +8..+0x28, the dwords at +0xfc / +0x1ac and the bytes at +0x104 / +0x1b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55C80, length 80 bytes.
// Leaf: Reset
// Callees: CScreenEffectPriorityStack::Reset @ 0x00C55B70. The chunk label (CMemoryFile::Open) is propagated; named by address. Shape: `this` in esi, one zero register, `ret`.
struct CScreenEffectPriorityStack {
    unsigned char _body[8];
    void Reset(long mode);                         // 0x00C55B70
};

struct CScreenEffectOwner_00c55c80 : CScreenEffectPriorityStack {
    long          m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28;   // +0x08..+0x28
    unsigned char _pad2c[0xd0];
    long          m_fc;                            // +0xfc
    unsigned char _pad100[4];
    bool          m_104;                           // +0x104
    unsigned char _pad105[0xa7];
    long          m_1ac;                           // +0x1ac
    bool          m_1b0;                           // +0x1b0
    void Reset();
};

void CScreenEffectOwner_00c55c80::Reset()
{
    CScreenEffectPriorityStack::Reset(1);
    m_8 = 0; m_c = 0; m_10 = 0; m_14 = 0; m_18 = 0; m_1c = 0; m_20 = 0; m_24 = 0; m_28 = 0;
    m_fc = 0;
    m_1ac = 0;
    m_104 = false;
    m_1b0 = false;
}