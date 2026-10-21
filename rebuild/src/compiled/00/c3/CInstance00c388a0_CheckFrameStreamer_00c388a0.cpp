// CInstance_00c388a0::CheckFrameStreamer() -- AI-library instance: log ".\CInstance.cpp(8) : Null frame streamer" when the frame streamer is missing and "(9) : ESTATE" when the state is not 2, then return the streamer's ready test (slot 1) chained into its slot-3 result
// Leaf: CheckFrameStreamer
// Retail 0x00C388A0, 384 bytes, EH frame (/EHsc), plain `ret`. Label: catalogued as CTCActivationReceptor::OnStateTransitionCheck
// (propagated); the body is AI steering-library code (the ".\CInstance.cpp" assert strings 0x012B542C), called once from
// 0x00C38F73; named by address.
// Callees: the library string's Append(const char*) 0x00C0DEC0 / AppendNumber(long) 0x00C0DF00 (`__fastcall` builders),
// ~string 0x00C0BA40, the empty trace temporary 0x00C16750; globals: the shared empty string 0x0139C900 and the optional
// log sink 0x0143BB2C. Strings "(" 0x0122E028, ") : " 0x012B1AE4, "Null frame streamer" 0x012B5418, "ESTATE" 0x012B5410,
// "\n" 0x0129B208.
// Layout (descriptive): frame streamer +0x16C (vtable slots 1 and 3), state +0xF8.
// Shape: the landed assert-stream model (CPointerArray_00c324f0::GetChecked 0x00C324F0): each check is a block holding the
// stack string (EH states 0 and 1) followed by its own empty trace temporary (two frame bytes); the zero ready result is
// returned through eax as is.
// Flags: /EHsc
extern char* g_pchNil_0139c900;                            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c388a0)(const char* text);
extern PFNLOGSINK_00c388a0 g_pfnLogSink_0143bb2c;          // 0x0143BB2C

struct CString_00c388a0 {
    char* m_pchData;
    CString_00c388a0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c388a0();                                   // 0x00C0BA40
    CString_00c388a0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c388a0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};
struct CTrace_00c388a0 {
    CTrace_00c388a0();                                     // 0x00C16750
};
struct CFrameStreamer_00c388a0 {
    virtual void v0();
    virtual long IsReady();                                // slot 1 (+0x04)
    virtual void v2();
    virtual long Stream();                                 // slot 3 (+0x0c)
};
struct CInstance_00c388a0 {
    unsigned char            _pad000[0xf8];
    long                     m_state;                      // +0xf8
    unsigned char            _pad0fc[0x16c - 0xfc];
    CFrameStreamer_00c388a0* m_pFrameStreamer;             // +0x16c
    long CheckFrameStreamer();
};

long CInstance_00c388a0::CheckFrameStreamer()
{
    if (!m_pFrameStreamer) {
        {
            CString_00c388a0 line;
            line.Append(".\\CInstance.cpp")->Append("(")->AppendNumber(8)->Append(") : ")->Append("Null frame streamer")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c388a0();
    }
    if (m_state != 2) {
        {
            CString_00c388a0 line;
            line.Append(".\\CInstance.cpp")->Append("(")->AppendNumber(9)->Append(") : ")->Append("ESTATE")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c388a0();
    }
    long ready = m_pFrameStreamer->IsReady();
    if (!ready)
        return ready;
    return m_pFrameStreamer->Stream();
}