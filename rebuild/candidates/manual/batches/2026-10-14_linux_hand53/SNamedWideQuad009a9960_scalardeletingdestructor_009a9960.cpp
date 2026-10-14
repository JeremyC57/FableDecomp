// SNamedWideQuad_009a9960::`scalar deleting destructor'(unsigned int flags) -- destroy the four CWideStrings of the member record at +4 (+0x10 down to +4; 0x0099B510) then the CCharString at +0 (0x0099EAE0), and free the record for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A9960, length 80 bytes.
// Leaf: ??_GSNamedWideQuad_009a9960@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: `this` in edi, the member record in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};
struct SWideQuad_009a9960 {                        // four wide strings
    CWideString m_0, m_4, m_8, m_c;
};
struct SNamedWideQuad_009a9960 {
    CCharString          m_name;                   // +0x00
    SWideQuad_009a9960   m_texts;                  // +0x04
    ~SNamedWideQuad_009a9960() { }
};

void Kill_009a9960(SNamedWideQuad_009a9960* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
