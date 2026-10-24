// CAutoDeletePointer::GetChecked() -- libpk's PKCAutoDelete.h accessor: an empty wrapper logs `PKCAutoDelete.h(32) : Should have checked first...` through a stack log line (shared buffer, optional sink) and raises the trace temporary; returns the held pointer either way
// Leaf: GetChecked
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17740, 186 bytes (+ int3 padding), __fastcall `ret`, EH-framed.
// Identity: a second instantiation of the landed CAutoDeletePointer::GetChecked @ 0x00C09920 (byte-identical body,
//   its own handler 0x01211EA8): `d:\dev\audio\ver_fable_1.5_b\libpk\PKCAutoDelete.h` line 32. Callees: the log line's
//   chained `__fastcall` appenders @ 0x00C0DEC0 (text) / 0x00C0DF00 (number), its destructor @ 0x00C0BA40, the
//   trace temporary @ 0x00C16750; globals: the shared buffer @ 0x0139C900, the sink @ 0x0143BB2C.
// Shape: the log line lives in its own block (state 0 -> -1 before the trace temporary, which lands on a dead frame
//   byte); `mov eax,fs:[0]` heads the frame set-up (VC7.1's order for this shape).
// Label correction: the catalog's `NScript_MurderTwist_CheckCondition` is propagated (no script code here).
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

struct CAutoDeletePointer {
    void* m_pObject;
    void* GetChecked();
};

void* CAutoDeletePointer::GetChecked()
{
    if (m_pObject == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32 << ") : " << "Should have checked first..." << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return m_pObject;
}