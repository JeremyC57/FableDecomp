// CCodaFrame::CCodaFrame(unsigned long channels, unsigned long samples) -- libpk audio frame (CFrame.cpp): store the channel and sample counts, zero the cursors, warn on zero channels / samples (lines 11, 12), allocate the 16-bit sample block (channels x samples) and warn EMEM (line 14) when it fails
// Leaf: ??0CCodaFrame@@QAE@KK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C491E0, 509 bytes, thiscall `ret 8`, /EHsc.
// Identity: libpk .\CFrame.cpp (lines 11, 12, 14); the landed CCodaFrame::Silence 0x00C49180 names this ctor and the layout.
// Callees: the trace stream helpers, CDebugBreak 0x00C16750, operator new[] 0x00BFEB22.
// Shape: members in the init list, member tests (the trace string lives in the dead argument slot).
void* __cdecl operator new[](unsigned int size);   // 0x00BFEB22 (MSVCR71 ??_U thunk)
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4c8b0)(const char* text);
extern PFNLOGSINK_00c4c8b0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

class CString { public:                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                       // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);   // 0x00C0DF00
};
struct CDebugBreak {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CDebugBreak();                         // 0x00C16750
};
struct CCodaFrame {
    unsigned long m_channels;                   // +0x00
    unsigned long m_samples;                    // +0x04 capacity in samples per channel
    unsigned long m_validSamples;               // +0x08
    unsigned long m_position;                   // +0x0c
    short*        m_pData;                      // +0x10 interleaved 16-bit PCM
    CCodaFrame(unsigned long channels, unsigned long samples);
};

CCodaFrame::CCodaFrame(unsigned long channels, unsigned long samples)
    : m_channels(channels), m_samples(samples), m_validSamples(0), m_position(0), m_pData(0)
{
    if (m_channels == 0) {
        {
            CString line;
            line << ".\\CFrame.cpp" << "(" << 11L << ") : " << "Expecting at least one channel!" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    if (m_samples == 0) {
        {
            CString line;
            line << ".\\CFrame.cpp" << "(" << 12L << ") : " << "Expecting frame to contain at least one sample!" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    m_pData = new short[m_samples * m_channels];
    if (m_pData == 0) {
        {
            CString line;
            line << ".\\CFrame.cpp" << "(" << 14L << ") : " << "EMEM" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
}