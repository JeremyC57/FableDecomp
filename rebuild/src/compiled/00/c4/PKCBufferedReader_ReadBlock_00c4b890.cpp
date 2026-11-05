// PKCBufferedReader::ReadBlock(void* buffer, unsigned long size) -- copy size bytes from the current position into buffer, warning when the request exceeds the internal buffers and refusing (false) a read past the end of the file
// Leaf: ?ReadBlock@PKCBufferedReader@@QAE_NPAXK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4B890, length 538 bytes, thiscall, `ret 8`, EH-framed (two CString states).
// Identity: libpk PKCBufferedReader (vtable 0x012B8458; both warnings name `.\PKCBufferedReader.cpp` lines 348 / 355);
//   the landed spelling ?ReadBlock@PKCBufferedReader@@QAE_NPAXK@Z (callers 0x00C3AA09 CReaderBase_00c4b300::Read and libpk's
//   vorbisfile read 0x00C89C5E; FINDINGS chunk 445 "CStreamReader ReadBlock").
// Callees: CString operator<< (const char*) 0x00C0DEC0 / (double) 0x00C0DED0 / (long) 0x00C0DF00 / (unsigned long) 0x00C0DF10,
//   ~CString 0x00C0BA40 (the landed libpk spellings); the block copier 0x00C4B640 (descriptive CopyOutOfBuffers: copies from
//   the resident blocks into the caller's buffer and returns the bytes copied; its messages "Error: alignment out" /
//   "Error: Expecting to be processing SOME bytes!"); g_pchNil_0139c900 / g_pfnLogSink_0143bb2c (held spellings, D4).
// Layout: the landed Seek 0x00C4AA40 model; +0x14 is the buffer block count the constructor sets (look-behind + look-ahead).
// Shape: the ratio is a float division (`fadd dword [2^32]` unsigned fix-ups, 0x0122DCB4) passed to operator<<(double).
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4aa40)(const char* text);
extern PFNLOGSINK_00c4aa40 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

class CString { public:
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                                         // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(double value);       // 0x00C0DED0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
    CString& __fastcall operator<<(unsigned long value);   // 0x00C0DF10
};

class PKCBufferedReader {                  // vtable 0x012B8458, 0x70 bytes
public:
    virtual void v00();                    // slot 0: 0x00C4AB90
    unsigned char* m_pBufferMemory;        // +4
    unsigned long m_blockSize;             // +8
    long m_c;
    unsigned long m_lookBehind;            // +0x10
    unsigned long m_numBlocks;             // +0x14
    unsigned char _pad18[0x8];
    unsigned long m_size;                  // +0x20
    unsigned long m_firstBlock;            // +0x24
    unsigned long m_28;
    unsigned long m_blockCount;            // +0x2c
    unsigned long m_position;              // +0x30
    unsigned long CopyOutOfBuffers(void* buffer, unsigned long size);   // 0x00C4B640
    bool ReadBlock(void* buffer, unsigned long size);
};

bool PKCBufferedReader::ReadBlock(void* buffer, unsigned long size)
{
    if (size == 0)
        return true;
    unsigned long capacity = m_numBlocks * m_blockSize;
    if (size > capacity) {
        CString line;
        line << ".\\PKCBufferedReader.cpp" << "(" << 348L << ") : " << "Warning: requesting a read which is "
             << (float)size / (float)capacity << " times the size of the internal buffers!!!" << "\n";
        if (g_pfnLogSink_0143bb2c)
            g_pfnLogSink_0143bb2c(line.m_pchData);
    }
    unsigned long position = m_firstBlock * m_blockSize + m_position;
    if (position + size > m_size) {
        CString line;
        line << ".\\PKCBufferedReader.cpp" << "(" << 355L << ") : " << "Warning: Attempt to read past the end of the file (requesting "
             << size << " bytes, at offset " << position << ", file length is " << m_size << ")" << "\n";
        if (g_pfnLogSink_0143bb2c)
            g_pfnLogSink_0143bb2c(line.m_pchData);
        return false;
    }
    do {
        unsigned long copied = CopyOutOfBuffers(buffer, size);
        size -= copied;
        buffer = (char*)buffer + copied;
    } while (size != 0);
    return true;
}