# h25_pkc.py: naming wave 25, family pkc -- 0x00C09920 PKCAutoDelete<...>::GetChecked (c692 queue).
#  The own row was landed as CAutoDeletePointer::GetChecked (Leaf "GetChecked", ?GetChecked@CAutoDeletePointer@@QAEPAXXZ). Its assert text
#  names libpk\PKCAutoDelete.h line 32 and its only caller 0x00C08D40 (CSystem::LoadBank, chunk 692) calls it on the same PKCAutoDelete
#  holder that Release 0x00C099F0 (landed ?Release@?$PKCAutoDelete@VCUnprovenPKObject_00c099f0@@@@QAE...) clears: the member of the
#  same template instantiation, ?GetChecked@?$PKCAutoDelete@VCUnprovenPKObject_00c099f0@@@@QAEPAVCUnprovenPKObject_00c099f0@@XZ
#  (libpk has no PDB: the template argument stays the 0x00C099F0 placeholder; GetChecked is the landed descriptive member name).
#  Re-authored as the class-template member, explicitly instantiated as the Release row is (name-only: same body).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
A = '00c09920'
he.edit(A, 'src', '''// CAutoDeletePointer::GetChecked() -- diagnose an empty pointer wrapper, then return its stored pointer
''', '''// PKCAutoDelete<CUnprovenPKObject_00c099f0>::GetChecked() -- libpk auto-delete pointer: diagnose an empty holder, then return its stored pointer
''')
he.edit(A, 'src', '''// Leaf: GetChecked
''', '''// Leaf: ?GetChecked@?$PKCAutoDelete@VCUnprovenPKObject_00c099f0@@@@QAEPAVCUnprovenPKObject_00c099f0@@XZ
// Naming wave 25 (c692 queue): the assert text names libpk\\PKCAutoDelete.h line 32 and the only caller 0x00C08D40 (CSystem::LoadBank)
//   calls it on the PKCAutoDelete holder that Release 0x00C099F0 (landed PKCAutoDelete<CUnprovenPKObject_00c099f0>::Release) clears, so it
//   is a member of that same template instantiation (libpk has no PDB: the template argument stays Release's placeholder). Was a
//   non-template CAutoDeletePointer::GetChecked returning void*; name-only (the body is unchanged).
''')
he.edit(A, 'src', '''struct CAutoDeletePointer {
    void* m_pObject;
    void* GetChecked();
};
void* CAutoDeletePointer::GetChecked() {
    if (m_pObject == 0) {
        {
            CString s(g_pDebugBuffer);
            s << "d:\\\\dev\\\\audio\\\\ver_fable_1.5_b\\\\libpk\\\\PKCAutoDelete.h" << "(" << 32 << ") : " << "Should have checked first..." << "\\n";
            if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return m_pObject;
}''', '''template <class T> class PKCAutoDelete {
public:
    T* m_pObject;                                        // +0x00
    T* GetChecked()
    {
        if (m_pObject == 0) {
            {
                CString s(g_pDebugBuffer);
                s << "d:\\\\dev\\\\audio\\\\ver_fable_1.5_b\\\\libpk\\\\PKCAutoDelete.h" << "(" << 32 << ") : " << "Should have checked first..." << "\\n";
                if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer);
            }
            CDebugBreak();
        }
        return m_pObject;
    }
};
class CUnprovenPKObject_00c099f0;
template class PKCAutoDelete<CUnprovenPKObject_00c099f0>;''')
