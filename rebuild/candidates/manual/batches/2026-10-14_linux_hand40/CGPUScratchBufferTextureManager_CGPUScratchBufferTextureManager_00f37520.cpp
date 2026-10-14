// CGPUScratchBufferTextureManager::CGPUScratchBufferTextureManager() -- the CBase base constructor, this class's vtable, then fifteen zero words at +4..+0x3c and a cleared flag byte at +0x40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F37520, length 80 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x01301CE4 = CGPUScratchBufferTextureManager (rtti-vtables.tsv). Shape: every store after the vtable store (a member
// object with an inline constructor), one zero register; `this` is returned.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct SScratchBufferState_00f37520 {              // fifteen words and a flag byte, all cleared
    long m_0, m_4, m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28, m_2c, m_30, m_34, m_38;
    bool m_flag;                                   // +0x3c
    SScratchBufferState_00f37520() : m_0(0), m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0), m_20(0), m_24(0), m_28(0), m_2c(0), m_30(0), m_34(0), m_38(0), m_flag(false) { }
};

struct CGPUScratchBufferTextureManager : CBase {   // vtable 0x01301CE4
    SScratchBufferState_00f37520 m_state;          // +0x04
    virtual void v00();
    CGPUScratchBufferTextureManager();
};

CGPUScratchBufferTextureManager::CGPUScratchBufferTextureManager()
    : CBase()
{
}
