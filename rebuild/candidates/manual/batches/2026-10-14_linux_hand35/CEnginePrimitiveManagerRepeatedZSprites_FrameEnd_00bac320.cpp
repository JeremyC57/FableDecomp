// CEnginePrimitiveManagerRepeatedZSprites::FrameEnd() -- shrink the unsigned vector at +0x80 to nothing (resize(0, zero)) and clear the count at +0xc0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAC320, length 43 bytes.
// Callees: vector<unsigned int>::resize(n, const T&) @ 0x00BAC990 (on the vector at +0x80; 0 and the fill value by reference). Shape: the fill value is a named
// local in the one-word frame (`push ecx`), its zero store scheduled after the pushes; the count store is an immediate; `this` stays in esi.
struct CRetailVector_uint {
    unsigned long* m_pFirst;
    unsigned long* m_pLast;
    unsigned long* m_pEnd;
    void resize(unsigned long count, const unsigned long& fill);   // 0x00BAC990
};

struct CEnginePrimitiveManagerRepeatedZSprites {
    unsigned char      _pad[0x80];
    CRetailVector_uint m_indices;                  // +0x80
    unsigned char      _pad8c[0x34];
    long               m_count;                    // +0xc0
    void FrameEnd();
};

void CEnginePrimitiveManagerRepeatedZSprites::FrameEnd()
{
    unsigned long fill = 0;
    m_indices.resize(0, fill);
    m_count = 0;
}
