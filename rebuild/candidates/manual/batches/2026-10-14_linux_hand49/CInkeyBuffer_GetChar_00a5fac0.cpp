// CInkeyBuffer::GetChar(unsigned short* character, unsigned char* flags) -- pop the oldest key of the 100-entry ring at +8 (read index +0x198, write index +0x19c) into the two out-pointers and advance the read index modulo 100; false when the ring is empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5FAC0, length 76 bytes.
// Shape: the read index re-read after each store, unsigned `div` by 100 (esi), `mov al,1` / `xor al,al`, `ret 8`.
struct SInkeyEntry_Retail { unsigned short m_char; unsigned char m_flags; unsigned char _pad; };   // 4 bytes
struct CInkeyBuffer {
    unsigned char      _pad[8];
    SInkeyEntry_Retail m_entries[100];             // +0x08 (a ring of 100)
    unsigned long      m_read;                     // +0x198
    unsigned long      m_write;                    // +0x19c
    bool GetChar(unsigned short* character, unsigned char* flags);
};

bool CInkeyBuffer::GetChar(unsigned short* character, unsigned char* flags)
{
    if (m_write != m_read) {
        *character = m_entries[m_read].m_char;
        *flags = m_entries[m_read].m_flags;
        m_read = (m_read + 1) % 100;
        return true;
    }
    return false;
}
