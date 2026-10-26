// CMemoryFile::SetLength(long length) -- grow the file: when the length exceeds the recorded one (+0xc) it is stored and the byte vector at +0x10 resized to it with zero fill (`resize(n, value)` @ 0x00486189; the zero byte is homed in the dead argument slot)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9730, length 48 bytes.
// Leaf: SetLength
// Shape: no frame, `this` stays in ecx (advanced to the vector), `ret 4`.
struct CRetailVector_009e9730 {
    unsigned char* m_first;
    unsigned char* m_last;
    unsigned char* m_end;
    void resize(unsigned long size, const unsigned char& value);   // 0x00486189
};

class CMemoryFile { public:
    unsigned char           _pad00[0xc];
    long                    Length;                // +0x0c
    CRetailVector_009e9730  m_buffer;              // +0x10
    void SetLength(long length);
};

void CMemoryFile::SetLength(long length)
{
    if (length > Length) {
        Length = length;
        m_buffer.resize(length, 0);
    }
}