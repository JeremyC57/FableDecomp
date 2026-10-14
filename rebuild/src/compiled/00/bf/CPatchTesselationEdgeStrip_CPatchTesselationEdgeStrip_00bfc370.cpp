// CPatchTesselationEdgeStrip::CPatchTesselationEdgeStrip() -- CBase constructed (0x0099A2F0), this class's vtable (0x012A8318, rtti CPatchTesselationEdgeStrip), then the state record at +4 is constructed inline: the eight dwords +8..+0x24, the two words at +4/+6 and the two bytes at +0x28/+0x29 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC370, length 58 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. The chunk label "CManagedTextureData::CManagedTextureData" is propagated (CManagedTextureData is 0x4c bytes with a counted pointer at +0x20; this 0x2c-byte record is the tesselation edge strip of vtable 0x012A8318). Shape: one zero register (eax) for every store, dwords first, then the words, then the bytes, `ret`.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

struct SEdgeStripState {                           // the strip's record at +4 (an inline constructor: its stores follow the vtable store)
    unsigned short m_firstVertex;                  // +0x04
    unsigned short m_vertexCount;                  // +0x06
    long           m_8;                            // +0x08
    long           m_c;                            // +0x0c
    long           m_10;                           // +0x10
    long           m_14;                           // +0x14
    long           m_18;                           // +0x18
    long           m_1c;                           // +0x1c
    long           m_20;                           // +0x20
    long           m_24;                           // +0x24
    bool           m_flag28;                       // +0x28
    bool           m_flag29;                       // +0x29
    SEdgeStripState()
    {
        m_8 = 0;
        m_c = 0;
        m_10 = 0;
        m_14 = 0;
        m_18 = 0;
        m_1c = 0;
        m_20 = 0;
        m_24 = 0;
        m_firstVertex = 0;
        m_vertexCount = 0;
        m_flag28 = false;
        m_flag29 = false;
    }
};
struct CPatchTesselationEdgeStrip : CBase {       // vtable 0x012A8318
    virtual void v00();
    SEdgeStripState m_state;                       // +0x04
    CPatchTesselationEdgeStrip();
};

CPatchTesselationEdgeStrip::CPatchTesselationEdgeStrip()
    : CBase(), m_state()
{
}