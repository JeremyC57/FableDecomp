// PKCBufferedReader::Seek(long offset, ESeekOrigin origin) -- the reader's fseek: turn an offset relative to the current position (0), the end (1) or the start (2) into an absolute byte offset; false for an unknown origin or a negative result, else the absolute Seek 0x00C4AA40
// Leaf: ?Seek@PKCBufferedReader@@QAE_NJW4ESeekOrigin@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4AFD0, 58 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 445 "0xC4AA20/0xC4AA30/0xC4AFD0/0xC4B890 = CStreamReader
//   Tell/GetSize/Seek/ReadBlock"):
//   - its callers are libpk's vorbisfile I/O (0x00C89C8E, and 0x00C8C8DD / 0x00C8C9BA, each followed by Tell 0x00C4AA20):
//     the fseek of the ov_callbacks-less vorbisfile, on the COggVorbisStream's reader (+4);
//   - `this` is PKCBufferedReader (vtable 0x012B8458; the constructor 0x00C4B300 and the absolute seek 0x00C4AA40 name
//     `.\PKCBufferedReader.cpp`); the end case uses the size +0x20 that GetSize 0x00C4AA30 returns, the current case the
//     position Tell 0x00C4AA20 computes (first resident block +0x24 times the block size +8, plus the in-window position
//     +0x30).
//   The manifest's ConvertOffsetToAbsolute is a guess (forge-autoname2); the function ends in the seek itself.
// Shape: `sub eax,0 ; je ; dec eax ; je ; dec eax ; jne default` is the 3-case switch; the current-position and end cases
//   share the `add eax,offset` tail, the start case takes the offset as is; `test eax,eax ; jge` is `absolute < 0`.
enum ESeekOrigin { SEEK_ORIGIN_CURRENT = 0, SEEK_ORIGIN_END = 1, SEEK_ORIGIN_START = 2 };

class PKCBufferedReader {                  // vtable 0x012B8458, 0x70 bytes
public:
    virtual void v00();                    // slot 0: 0x00C4AB90
    unsigned char* m_pBufferMemory;        // +4
    unsigned long m_blockSize;             // +8
    long m_c;
    unsigned long m_lookBehind;            // +0x10
    unsigned char _pad14[0xc];
    unsigned long m_size;                  // +0x20
    unsigned long m_firstBlock;            // +0x24
    unsigned long m_28;
    unsigned long m_blockCount;            // +0x2c
    unsigned long m_position;              // +0x30
    bool Seek(unsigned long offset);       // 0x00C4AA40
    bool Seek(long offset, ESeekOrigin origin);
};

bool PKCBufferedReader::Seek(long offset, ESeekOrigin origin)
{
    long absolute;
    switch (origin) {
    case SEEK_ORIGIN_CURRENT:
        absolute = m_firstBlock * m_blockSize + m_position + offset;
        break;
    case SEEK_ORIGIN_END:
        absolute = m_size + offset;
        break;
    case SEEK_ORIGIN_START:
        absolute = offset;
        break;
    default:
        return false;
    }
    if (absolute < 0)
        return false;
    return Seek((unsigned long)absolute);
}
