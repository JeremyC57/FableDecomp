// PKCBufferedReader::ConvertOffsetToAbsolute(long offset, ESeekOrigin origin) -- turns an offset relative to the current position (0), the end (1) or the start (2) into an absolute byte offset; false for an unknown origin or a negative result, else seeks there
// Leaf: ConvertOffsetToAbsolute
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4AFD0, 58 bytes (+ int3 padding), __fastcall, `ret 8`.
// Callees: the absolute seek 0x00C4AA40 (landed as CBufferedReader_00c4aa40::Seek, `ret 4`, bool).
// Shape: `sub eax,0 ; je ; dec eax ; je ; dec eax ; jne default` is the 3-case switch; the current-position case
//   (first resident block +0x24 times the block size +8, plus the in-window position +0x30) and the end case (size +0x20)
//   share the `add eax,offset` tail, the start case takes the offset as is; `test eax,eax ; jge` is `absolute < 0`.
// Layout as in the landed absolute seek: block size +8, size +0x20, first resident block +0x24, window position +0x30.
// Name: the manifest label (ConvertOffsetToAbsolute) fits the body (relative offset to absolute, then the absolute seek).
enum ESeekOrigin { SEEK_ORIGIN_CURRENT = 0, SEEK_ORIGIN_END = 1, SEEK_ORIGIN_START = 2 };

struct PKCBufferedReader {
    long m_0;
    long m_4;
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
    bool ConvertOffsetToAbsolute(long offset, ESeekOrigin origin);
};

bool PKCBufferedReader::ConvertOffsetToAbsolute(long offset, ESeekOrigin origin)
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
    return Seek(absolute);
}