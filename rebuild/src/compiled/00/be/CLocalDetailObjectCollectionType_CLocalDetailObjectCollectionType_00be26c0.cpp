// CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType(const CLocalDetailObjectCollectionType& other) -- copy constructor: the CBaseClassNonCopyable base default-constructed (0x0099A2F0), this class's vtable (0x012A7D94), the seven dwords +4..+0x1c copied memberwise and the 13-dword block at +0x20 copied as a block (`rep movsd`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE26C0, length 83 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Shape: `this` in ebx, the source in eax, the dwords moved in pairs through ecx/edx, `ret 4`.
struct SCollectionBlock_00be2680 {                 // 0x34 bytes of plain data at +0x20
    long m_words[13];
};

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};

class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable { public:   // vtable 0x012A7D94
    long                      m_4;                 // +0x04
    long                      m_8;                 // +0x08
    long                      m_c;                 // +0x0c
    long                      m_10;                // +0x10
    long                      m_14;                // +0x14
    long                      m_18;                // +0x18
    long                      m_1c;                // +0x1c
    SCollectionBlock_00be2680 m_block;             // +0x20
    CLocalDetailObjectCollectionType(const CLocalDetailObjectCollectionType& other);
};

CLocalDetailObjectCollectionType::CLocalDetailObjectCollectionType(const CLocalDetailObjectCollectionType& other)
    : CBaseClassNonCopyable()
{
    m_14 = other.m_14;
    m_1c = other.m_1c;
    m_18 = other.m_18;
    m_10 = other.m_10;
    m_4 = other.m_4;
    m_c = other.m_c;
    m_8 = other.m_8;
    m_block = other.m_block;
}