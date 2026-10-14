// CInkeyBuffer::AddChar(unsigned short character, unsigned char flags) -- append a key to the 100-entry ring at +8 (write index +0x19c, read index +0x198): dropped when the ring is full (the next write index would meet the read index), else the character and flags stored and the write index advanced modulo 100
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5FA70, length 80 bytes.
// Shape: unsigned `div` by 100 (edi) twice, the write index re-read for each store, `ret 8`.
struct SInkeyEntry_Retail { unsigned short m_char; unsigned char m_flags; unsigned char _pad; };   // 4 bytes
struct CInkeyBuffer {
    unsigned char      _pad[8];
    SInkeyEntry_Retail m_entries[100];             // +0x08 (a ring of 100)
    unsigned long      m_read;                     // +0x198
    unsigned long      m_write;                    // +0x19c
    void AddChar(unsigned short character, unsigned char flags);
};

void CInkeyBuffer::AddChar(unsigned short character, unsigned char flags)
{
    if ((m_write + 1) % 100 != m_read) {
        m_entries[m_write].m_char = character;
        m_entries[m_write].m_flags = flags;
        m_write = (m_write + 1) % 100;
    }
}