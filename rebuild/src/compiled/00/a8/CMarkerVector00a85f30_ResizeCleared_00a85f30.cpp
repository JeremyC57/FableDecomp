// CMarkerVector_00a85f30::ResizeCleared(unsigned int count) -- resize the marker vector to `count` copies of a zeroed 0x28-byte marker (nine words and a flag byte at +0x25), then destroy the template marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A85F30, length 69 bytes.
// Callees: vector<marker>::resize(unsigned int, const marker&) @ 0x00A85EC0 (on `this`), the marker's destructor @ 0x00A851E0 (on the local; manifest
// "CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures" is propagated). Shape: the nine words and the byte are zeroed through eax before the pushes
// (an inline constructor), the destructor runs on the local after the resize returns. The chunk label "CEngine::ClipBox" is propagated; the class is named
// by address.
struct SMarker_00a85f30 {
    long          m_words[9];                      // +0x00 .. +0x24
    unsigned char _pad24;                          // +0x24
    unsigned char m_flag;                          // +0x25
    unsigned char _pad26[2];
    SMarker_00a85f30()
    {
        m_words[0] = 0; m_words[1] = 0; m_words[2] = 0; m_words[3] = 0; m_words[4] = 0;
        m_words[5] = 0; m_words[6] = 0; m_words[7] = 0; m_words[8] = 0;
        m_flag = 0;
    }
    ~SMarker_00a85f30();                           // 0x00A851E0
};

struct CMarkerVector_00a85f30 {
    void resize(unsigned int count, const SMarker_00a85f30& value);   // 0x00A85EC0
    void ResizeCleared(unsigned int count);
};

void CMarkerVector_00a85f30::ResizeCleared(unsigned int count)
{
    SMarker_00a85f30 blank;
    resize(count, blank);
}