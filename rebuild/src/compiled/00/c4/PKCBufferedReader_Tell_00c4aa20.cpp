// PKCBufferedReader::Tell() -- the reader's absolute stream position: current block index (+0x24) times the block size (+0x8), plus the offset inside the block (+0x30)
// Leaf: ?Tell@PKCBufferedReader@@QAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4AA20, 11 bytes, thiscall, plain `ret`.
// Identity: FINDINGS chunk 445 ("0xC4AA20/0xC4AA30/0xC4AFD0/0xC4B890 = CStreamReader Tell/GetSize/Seek/ReadBlock"), the
//   class of the landed PKCBufferedReader::GetSize 0x00C4AA30 / Seek 0x00C4AA40 (.\PKCBufferedReader.cpp, block size +0x8);
//   callers: libpk's vorbisfile I/O 0x00C89C37, 0x00C8C8E4, 0x00C8C9C5 (the tell callback). The catalog label
//   "CGameMap::GetMemoryFootprint" is propagated. Descriptive member names for +0x24 / +0x30 (libpk, no PDB).
// Shape: `m_block * m_blockSize + m_offset` (`mov eax,[ecx+24h] ; imul eax,[ecx+8] ; add eax,[ecx+30h]`).
class PKCBufferedReader {                  // vtable 0x012B8458 (.\PKCBufferedReader.cpp)
public:
    virtual void v00();
    unsigned char* m_pBufferMemory;        // +0x04
    unsigned long  m_blockSize;            // +0x08
    unsigned long  m_0c;                   // +0x0c
    unsigned long  m_lookBehind;           // +0x10
    unsigned long  m_bufferCount;          // +0x14
    void*          m_pSource;              // +0x18
    unsigned char* m_pBufferStates;        // +0x1c
    unsigned long  m_size;                 // +0x20
    unsigned long  m_currentBlock;         // +0x24
    unsigned long  m_28;                   // +0x28
    unsigned long  m_2c;                   // +0x2c
    unsigned long  m_blockOffset;          // +0x30
    unsigned long Tell();
};

unsigned long PKCBufferedReader::Tell()
{
    return m_currentBlock * m_blockSize + m_blockOffset;
}