// PKCBufferedReader::GetSize() -- the stream size in bytes the buffered reader serves (+0x20)
// Leaf: ?GetSize@PKCBufferedReader@@QAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4AA30, 4 bytes (`mov eax,[ecx+20h] ; ret`, + int3 padding), thiscall.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 445 "0xC4AA20/0xC4AA30/0xC4AFD0/0xC4B890 = CStreamReader
//   Tell/GetSize/Seek/ReadBlock"):
//   - +0x20 is PKCBufferedReader's size: its absolute Seek 0x00C4AA40 fails past it (`offset > m_size`), its relative
//     Seek 0x00C4AFD0 measures the end origin from it, its ReadBlock 0x00C4B890 warns when a read would pass it;
//   - callers: libpk's vorbisfile I/O 0x00C89C40, and the landed 0x00C3AA30 (tail jump) / 0x00C3AA50 (twice), the
//     reader wrapper around PKCBufferedReader (its constructor 0x00C3AAC0 calls the reader constructor 0x00C4B300).
//   The landed `DrawGetEnvironment` (__fastcall over raw bytes) is a propagated manifest label.
class PKCBufferedReader {                  // vtable 0x012B8458, 0x70 bytes (.\PKCBufferedReader.cpp)
public:
    virtual void v00();                    // slot 0: 0x00C4AB90
    unsigned char* m_pBufferMemory;        // +0x04
    unsigned long  m_blockSize;            // +0x08
    unsigned long  m_0c;                   // +0x0c
    unsigned long  m_lookBehind;           // +0x10
    unsigned long  m_bufferCount;          // +0x14
    void*          m_pSource;              // +0x18
    unsigned char* m_pBufferStates;        // +0x1c
    unsigned long  m_size;                 // +0x20
    unsigned long GetSize();
};

unsigned long PKCBufferedReader::GetSize()
{
    return m_size;
}
