// CAutoDeletePointer::GetChecked() -- diagnose an empty pointer wrapper, then return its stored pointer
// Retail 0x00C09920, 186 bytes. Caller 0x00C08F0D invokes this on a stack wrapper;
// it consumes this return as a checked auto-delete pointer. The propagated CWorldManager::OnShutdown catalog
// label is not supported by the PKCAutoDelete.h assertion string or call shape. /EHsc emits stream cleanup.
// Leaf: GetChecked
// Flags: /EHsc
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value); // 0x00C0DF00
    ~CDebugStream(); // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); }; // 0x00C16750
extern char* g_pDebugBuffer; // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*); // 0x0143BB2C
struct CAutoDeletePointer {
    void* m_pObject;
    void* GetChecked();
};
void* CAutoDeletePointer::GetChecked() {
    if (m_pObject == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32 << ") : " << "Should have checked first..." << "\n";
            if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return m_pObject;
}