// CMultiStaticMeshEntryDef::CMultiStaticMeshEntryDef(const CMultiStaticMeshEntryDef& other) -- copy constructor: the CDefClassBase base copied (0x0046644C), this class's vtable (0x012438A4), then the dword at +0x28, the two bytes +0x2c / +0x2d and the dwords +0x30 / +0x34 copied memberwise
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EB864, length 57 bytes.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C. Shape: the size-optimised module (every member through eax/al), the source in edi, `this` in esi, `ret 4`.
struct CDefClassBase {                             // 0x28 bytes
    virtual void v00();
    unsigned char _body[0x24];
    CDefClassBase(const CDefClassBase& other);     // 0x0046644C
};

struct CMultiStaticMeshEntryDef : CDefClassBase { // vtable 0x012438A4
    long          m_28;                            // +0x28
    bool          m_2c;                            // +0x2c
    bool          m_2d;                            // +0x2d
    long          m_30;                            // +0x30
    long          m_34;                            // +0x34
    CMultiStaticMeshEntryDef(const CMultiStaticMeshEntryDef& other);
};

CMultiStaticMeshEntryDef::CMultiStaticMeshEntryDef(const CMultiStaticMeshEntryDef& other)
    : CDefClassBase(other)
{
    m_28 = other.m_28;
    m_2c = other.m_2c;
    m_2d = other.m_2d;
    m_30 = other.m_30;
    m_34 = other.m_34;
}
