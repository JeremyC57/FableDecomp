// C16BitSamplePtr::C16BitSamplePtr(short* pSamples, long channels) -- libpk mixer: a cursor over interleaved 16-bit samples; store the buffer and the channel count, asserting (libpk log line + trace break) that the buffer is not null and that there is at least one channel
// Leaf: ??0C16BitSamplePtr@@QAE@PAFJ@Z
// Flags: /EHsc
// Grades: 1 (RELOCATION_MATCH first grade)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62A50, 337 bytes, thiscall `ret 8` (returns this), EH-framed (handler 0x01216470).
// Identity: the assertion text names the file `.\C16BitSamplePtr.cpp` (0x012B9374) with lines 8 and 9 ("Null buffer"
//   0x012B9368, "Expecting at least 1 channel!" 0x012B9348); the neighbouring 0x00C62A30 advances the same two words
//   (`m_pSamples += n * m_channels`, 16-bit samples). libpk has no PDB: class name from the file name, member and
//   parameter names descriptive. Caller: the libpk mixer at 0x00C48039 (a stack cursor). Catalog label
//   "C16BitSamplePtr_Initialize" -> constructor.
// Callees: the libpk log line (landed spellings): CDebugStream's `__fastcall` appenders 0x00C0DEC0 (text) / 0x00C0DF00
//   (number), ~CDebugStream 0x00C0BA40, the trace temporary CDebugBreak() 0x00C16750; globals g_pDebugBuffer 0x0139C900
//   and the report sink g_pfnErrorReport 0x0143BB2C.
// Shape: the members are initialised before the checks (init list); each assertion is the libpk log-line block (the
//   stream in its own scope: state 0/1 -> -1 before its destructor) followed by the CDebugBreak temporary, as the landed
//   PKCAutoFile::GetChecked 0x00C31DB0.
struct CDebugStream {                      // 4 bytes: the message text (the library CString over the shared nil buffer)
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);         // 0x00C0DF00
    ~CDebugStream();                                         // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                       // 0x00C16750
extern char* g_pDebugBuffer;                                 // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);     // 0x0143BB2C

class C16BitSamplePtr {
public:
    short* m_pSamples;                                       // +0x00
    long   m_channels;                                       // +0x04
    C16BitSamplePtr(short* pSamples, long channels);
};

C16BitSamplePtr::C16BitSamplePtr(short* pSamples, long channels)
    : m_pSamples(pSamples), m_channels(channels)
{
    if (m_pSamples == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\C16BitSamplePtr.cpp" << "(" << 8 << ") : " << "Null buffer" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    if (m_channels == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\C16BitSamplePtr.cpp" << "(" << 9 << ") : " << "Expecting at least 1 channel!" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
}