// StreamReader::AdvancePositionRelative(long offset) -- move the read cursor of an in-memory byte stream by a signed offset, refusing to run past the end or before the start
// Leaf: AdvancePositionRelative
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C426F0, 35 bytes (+ int3 padding).
// Only caller: the RIFF chunk walker @ 0x00C427D0 (beside CCompressorZlib::ParseWaveHeader
// 0x00C42850), which skips each chunk's (word-aligned) size and then tests position < size.
// The class name is the manifest's; its layout is modelled from the bytes: cursor at +0x8, size at
// +0xc (both unsigned: the end test is `jbe`), the buffer pointer before them.
// Shape: the offset argument itself becomes the new cursor (`offset += m_position`: a separate local
// swaps eax/edx); a forward move fails when the new cursor passes the size (unsigned), a backward move when
// it goes negative (`js`); success stores the cursor and returns true (`mov al,1`).
class StreamReader {
public:
    const unsigned char* m_pBuffer;             // +0x0
    unsigned long m_flags;                      // +0x4
    unsigned long m_position;                   // +0x8
    unsigned long m_size;                       // +0xc
    bool AdvancePositionRelative(long offset);
};

bool StreamReader::AdvancePositionRelative(long offset)
{
    if (offset >= 0) {
        offset += m_position;
        if ((unsigned long)offset > m_size)
            return false;
    } else {
        offset += m_position;
        if (offset < 0)
            return false;
    }
    m_position = offset;
    return true;
}