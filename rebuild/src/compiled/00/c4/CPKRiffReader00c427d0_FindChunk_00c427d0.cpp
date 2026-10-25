// CPKRiffReader_00c427d0::FindChunk(SRiffChunk_00c427d0* pChunk, bool fromStart) -- optionally rewind to the first sub-chunk (offset 12, past the RIFF header), then walk the 8-byte chunk headers until one carries pChunk's id: record its size (padded to even when skipping) and, on a hit, the data offset; false at the end of the data or when a skip fails
// Leaf: ?FindChunk@CPKRiffReader_00c427d0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C427D0, 120 bytes (+ int3 padding), __fastcall `ret 8`, bool in al.
// Library: libpk audio (no PDB/RTTI); descriptive names. The reader keeps its vtable at +0 (slot 3 = Read(dst,
//   bytes), the memory reader's is 0x00C42720), the position at +8 and the data size at +0xc; Seek(long delta)
//   0x00C426F0 moves the position relative and fails outside [0, size]. Callers 0x00C428D7, 0x00C42943, 0x00C4298D
//   (the WAV loader). The manifest's "StreamReader_SearchByteInRange" is propagated.
struct SRiffChunk_00c427d0 {
    unsigned long m_id;                                 // +0
    unsigned long m_size;                               // +4
    unsigned long m_offset;                             // +8
};

struct SRiffHeader_00c427d0 {
    unsigned long m_id;
    unsigned long m_size;
};

class CPKRiffReader_00c427d0 {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool Read(void* pDest, unsigned long bytes);   // slot 3
    bool Seek(long delta);                              // 0x00C426F0
    bool FindChunk(SRiffChunk_00c427d0* pChunk, bool fromStart);
private:
    void* m_pData;                                      // +4
    unsigned long m_position;                           // +8
    unsigned long m_size;                               // +0xc
};

bool CPKRiffReader_00c427d0::FindChunk(SRiffChunk_00c427d0* pChunk, bool fromStart)
{
    if (fromStart)
        m_position = 12;
    while (m_position < m_size) {
        SRiffHeader_00c427d0 header;
        Read(&header, 8);
        pChunk->m_size = header.m_size;
        if (pChunk->m_id == header.m_id) {
            pChunk->m_offset = m_position;
            return true;
        }
        if (pChunk->m_size & 1)
            pChunk->m_size++;
        if (!Seek(pChunk->m_size))
            return false;
    }
    return false;
}