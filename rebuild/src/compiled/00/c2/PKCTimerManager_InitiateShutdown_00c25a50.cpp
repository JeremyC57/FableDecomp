// PKCTimerManager::InitiateShutdown() -- warn "Shutdown already initiated..." (PKCTimerManager.cpp line 33) when the shutdown flag is already set, then set it
// Flags: /EHsc
// Leaf: InitiateShutdown
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C25A50, 190 bytes (`ret`), built under /EHsc. Descriptive member names (no PDB
//   covers the libpk audio library; the warning names the source file ".\PKCTimerManager.cpp"): the manifest's
//   "PKCTimerManager_Close" is close; the body only raises the shutdown flag at +4. Its only caller 0x00C25C20 calls
//   it first and then destroys the worker thread object at +0x14 (created by the constructor 0x00C25B10).
// Callees: the library log line (MFC-style CString over the shared empty buffer @ 0x0139C900): Append(const char*)
//   @ 0x00C0DEC0 and AppendNumber(long) @ 0x00C0DF00 (both `__fastcall`, returning the string), ~CString
//   @ 0x00C0BA40; the optional log sink @ 0x0143BB2C (`__fastcall`, the text); CDebugBreak @ 0x00C16750 (an empty
//   temporary homed in the dead high byte of the log-line frame word, `lea ecx,[esp+7]`).
// Shape: the library's warning block (`{ CString line; line.Append(__FILE__)->...->Append("\n"); if (sink)
//   sink(line); } CDebugBreak();`) inside `if (m_bShutdown)`; the flag store `mov byte ptr [esi+4],1` is scheduled
//   after the EH-frame restore load.

typedef void (__fastcall* PFNLOGSINK)(const char* text);
extern PFNLOGSINK g_pfnLogSink;                 // 0x0143BB2C
extern char* g_pchEmptyString;                  // 0x0139C900 (the shared empty string buffer)

class CString {                                 // the library's MFC-style string (one word)
public:
    char* m_pchData;
    CString() : m_pchData(g_pchEmptyString) {}
    ~CString();                                 // 0x00C0BA40
    CString* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

struct PKCTimerThread;

struct PKCTimerManager {
    void*           m_vtable;                   // +0x00
    bool            m_bShutdown;                // +0x04
    unsigned char   _pad05[0x14 - 0x05];
    PKCTimerThread* m_pThread;                  // +0x14
    void InitiateShutdown();
};

void PKCTimerManager::InitiateShutdown()
{
    if (m_bShutdown) {
        {
            CString line;
            line.Append(".\\PKCTimerManager.cpp")->Append("(")->AppendNumber(33)->Append(") : ")->Append("Shutdown already initiated...")->Append("\n");
            if (g_pfnLogSink)
                g_pfnLogSink(line.m_pchData);
        }
        CDebugBreak();
    }
    m_bShutdown = true;
}