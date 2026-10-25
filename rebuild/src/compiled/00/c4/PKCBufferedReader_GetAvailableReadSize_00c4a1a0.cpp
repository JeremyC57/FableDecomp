// PKCBufferedReader::GetAvailableReadSize(unsigned long position, long size) -- how much of a granularity-aligned read of size bytes at position fits before the end of the stream: all of it, else what is left (asserting the request is aligned, overruns by less than one granule and leaves a positive size)
// Leaf: ?GetAvailableReadSize@PKCBufferedReader@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4A1A0, 523 bytes (+ int3 padding), `ret 8`. Built under /EHsc (one EH state
//   per assertion's log line: 0, 1, 2).
// Callees: the libpk log-line string (as the landed PKCBufferedReader Seek 0x00C4AA40: default constructed from the
//   shared empty data @ 0x0139C900, Append(const char*) @ 0x00C0DEC0 / AppendNumber(long) @ 0x00C0DF00 (`__fastcall`
//   members), ~CString @ 0x00C0BA40), the optional log sink @ 0x0143BB2C and the CTrace debug-break temporary
//   @ 0x00C16750. Literals: ".\PKCBufferedReader.cpp" lines 97 / 101 / 102 with "Error: should be aligned to
//   granularity", "Error: Should not have primed this buffer...", "Error: Cannot return a negative size!!".
// Layout: the landed reader model: granularity (block size) +0x08, stream size +0x20.
// Shape: each check is the libpk assertion macro (a block-scoped log line handed to the sink, then the trace
//   temporary); the alignment test is unsigned (`div`), the overrun tests signed (`jl` / `jg`): the remaining size is
//   a long, the granularity compared through a long cast.
extern char* g_pchNil_0139c900;                         // 0x0139C900
typedef void (__fastcall* PFNLOGSINK)(const char* text);
extern PFNLOGSINK g_pfnLogSink_0143bb2c;                // 0x0143BB2C

struct CString {                                        // MFC-style string: one word
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                                         // 0x00C0BA40
    CString* __fastcall Append(const char* text);       // 0x00C0DEC0
    CString* __fastcall AppendNumber(long value);       // 0x00C0DF00
};

struct CTrace {                                         // empty debug-break temporary
    CTrace();                                           // 0x00C16750
};

#define PK_ASSERT_MSG(cond, line_no, message)                                                                  \
    if (!(cond)) {                                                                                               \
        {                                                                                                        \
            CString line;                                                                                        \
            line.Append(".\\PKCBufferedReader.cpp")->Append("(")->AppendNumber(line_no)->Append(") : ")->Append(message)->Append("\n"); \
            if (g_pfnLogSink_0143bb2c)                                                                           \
                g_pfnLogSink_0143bb2c(line.m_pchData);                                                           \
        }                                                                                                        \
        CTrace();                                                                                                \
    }

class PKCBufferedReader {
public:
    long          m_0;
    long          m_4;
    unsigned long m_blockSize;                          // +0x08 (the read granularity)
    unsigned char _pad0c[0x20 - 0x0c];
    unsigned long m_size;                               // +0x20 (the stream size)
    long GetAvailableReadSize(unsigned long position, long size);
};

long PKCBufferedReader::GetAvailableReadSize(unsigned long position, long size)
{
    PK_ASSERT_MSG(size % m_blockSize == 0, 97, "Error: should be aligned to granularity")
    long remaining = m_size - position - size;
    if (remaining >= 0)
        return size;
    long overrun = -remaining;
    PK_ASSERT_MSG(overrun < (long)m_blockSize, 101, "Error: Should not have primed this buffer...")
    PK_ASSERT_MSG(size > overrun, 102, "Error: Cannot return a negative size!!")
    return remaining + size;
}