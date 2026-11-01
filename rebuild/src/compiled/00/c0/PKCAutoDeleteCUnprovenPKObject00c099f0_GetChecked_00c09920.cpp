// PKCAutoDelete<CUnprovenPKObject_00c099f0>::GetChecked() -- libpk auto-delete pointer: diagnose an empty holder, then return its stored pointer
// Retail 0x00C09920, 186 bytes. Caller 0x00C08F0D invokes this on a stack wrapper;
// it consumes this return as a checked auto-delete pointer. The propagated CWorldManager::OnShutdown catalog
// label is not supported by the PKCAutoDelete.h assertion string or call shape. /EHsc emits stream cleanup.
// Leaf: ?GetChecked@?$PKCAutoDelete@VCUnprovenPKObject_00c099f0@@@@QAEPAVCUnprovenPKObject_00c099f0@@XZ
// Naming wave 25 (c692 queue): the assert text names libpk\PKCAutoDelete.h line 32 and the only caller 0x00C08D40 (CSystem::LoadBank)
//   calls it on the PKCAutoDelete holder that Release 0x00C099F0 (landed PKCAutoDelete<CUnprovenPKObject_00c099f0>::Release) clears, so it
//   is a member of that same template instantiation (libpk has no PDB: the template argument stays Release's placeholder). Was a
//   non-template CAutoDeletePointer::GetChecked returning void*; name-only (the body is unchanged).
// Flags: /EHsc
class CString { public:
    char* m_pBuffer;
    CString(char* buffer) : m_pBuffer(buffer) { }
    CString& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CString& __fastcall operator<<(long value); // 0x00C0DF00
    ~CString(); // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); }; // 0x00C16750
extern char* g_pDebugBuffer; // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*); // 0x0143BB2C
template <class T> class PKCAutoDelete {
public:
    T* m_pObject;                                        // +0x00
    T* GetChecked()
    {
        if (m_pObject == 0) {
            {
                CString s(g_pDebugBuffer);
                s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32 << ") : " << "Should have checked first..." << "\n";
                if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer);
            }
            CDebugBreak();
        }
        return m_pObject;
    }
};
class CUnprovenPKObject_00c099f0;
template class PKCAutoDelete<CUnprovenPKObject_00c099f0>;