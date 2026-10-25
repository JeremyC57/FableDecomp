// PKCAutoFile::GetChecked() -- libpk's PKCAutoFile.cpp accessor: an empty file wrapper logs `.\PKCAutoFile.cpp(26) : Should have checked first...` through a stack log line (shared buffer, optional sink) and raises the trace temporary; returns the held file pointer either way
// Leaf: GetChecked
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31DB0, 186 bytes, __fastcall `ret`, EH-framed (handler 0x01214178).
// Identity: the PKCAutoFile.cpp twin of CAutoDeletePointer::GetChecked (landed 0x00C09920, chunk-433 0x00C17740):
//   the same log-line shape with `.\PKCAutoFile.cpp` line 26 (0x1A). Callers: the libpk bank loader 0x00C174A0 (four
//   calls at 0x00C1735A..0x00C175B8) reads the open bank file through it.
// Callees: the log line's chained `__fastcall` appenders @ 0x00C0DEC0 (text) / 0x00C0DF00 (number), its destructor
//   @ 0x00C0BA40, the trace temporary @ 0x00C16750; globals: the shared buffer @ 0x0139C900, the sink @ 0x0143BB2C.
// Shape: the log line lives in its own block (state 0 -> -1 before the trace temporary, which lands on a dead frame
//   byte); `mov eax,fs:[0]` heads the frame set-up (VC7.1's order for this shape).
// Label correction: the catalog's `CBankFile_ValidateOpen` is propagated (descriptive name; no PDB covers libpk).
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

struct PKCAutoFile {
    void* m_pFile;
    void* GetChecked();
};

void* PKCAutoFile::GetChecked()
{
    if (m_pFile == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\PKCAutoFile.cpp" << "(" << 26 << ") : " << "Should have checked first..." << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return m_pFile;
}