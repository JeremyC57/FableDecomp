// STextRecord_00543530::operator=(const STextRecord_00543530&) -- member-wise copy of a 0x2c-byte record: two 12-byte triples (each through its own implicit assignment), three longs, a CWideString at +0x24 (out of line), a trailing long
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00543530, length 91 bytes.
// Callees: CWideString::operator= @ 0x0099B7D0 (on +0x24, the other's +0x24 by reference). Shape: the first triple is copied with the other's address in eax
// (`mov eax,edi ; mov edx,[eax] ; mov [ecx],edx ...`), the second with `lea ecx,[edi+0Ch]` / `lea edx,[esi+0Ch]` (its implicit operator= inlined with `this` in
// edx), then the three longs, the string, the last long, and `this` is returned. The chunk label "CEnginePrimitive2DText::operator=" is propagated (the 0x0054xxxx
// module is game code); the record is named by address.
struct CWideString {
    void* m_pData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

struct STriple_00543530 { long m_x, m_y, m_z; };

struct STextRecord_00543530 {
    STriple_00543530 m_position;                   // +0x00
    STriple_00543530 m_size;                       // +0x0c
    long             m_a;                          // +0x18
    long             m_b;                          // +0x1c
    long             m_c;                          // +0x20
    CWideString      m_text;                       // +0x24
    long             m_d;                          // +0x28
    STextRecord_00543530& operator=(const STextRecord_00543530& other);
};

STextRecord_00543530& STextRecord_00543530::operator=(const STextRecord_00543530& other)
{
    m_position = other.m_position;
    m_size = other.m_size;
    m_a = other.m_a;
    m_b = other.m_b;
    m_c = other.m_c;
    m_text = other.m_text;
    m_d = other.m_d;
    return *this;
}