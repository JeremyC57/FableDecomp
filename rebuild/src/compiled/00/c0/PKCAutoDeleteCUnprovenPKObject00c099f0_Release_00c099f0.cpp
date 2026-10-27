// PKCAutoDelete<CUnprovenPKObject_00c099f0>::Release() -- libpk auto-delete pointer: warn when it is empty ("Shouls have checked first..."), then give the owned pointer away and clear it
// Leaf: ?Release@?$PKCAutoDelete@VCUnprovenPKObject_00c099f0@@@@QAEPAVCUnprovenPKObject_00c099f0@@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C099F0, 192 bytes, thiscall, plain `ret`, EH frame (state 0 = the message string).
// Identity (descriptive): the assertion text names d:\dev\audio\ver_fable_1.5_b\libpk\PKCAutoDelete.h line 42 and the
//   body hands out and clears the one owned pointer at +0 (release semantics); libpk is outside every PDB, so the template
//   argument is the placeholder CUnprovenPKObject_00c099f0 and the member name Release is descriptive. Only caller 0x00C08D40 (manifest _global::CSystem_ProcessMainEvent);
//   the caller keeps the result and later destroys its own copy through vtable slot 6 (+0x18, a deleting destructor).
// Callees: CString::operator<<(const char*) 0x00C0DEC0 / operator<<(long) 0x00C0DF00 (__fastcall), CString::~CString
//   0x00C0BA40, CDebugBreak::CDebugBreak 0x00C16750; the assert hook pointer 0x0143BB2C; the nil string 0x0139C900.
// Shape (family K058, representative 0x00C099F0): the libpk assert macro as the landed PKCAutoDeleteArray accessor 0x00C2EBC0
//   (a nested block for the string, the CDebugBreak temporary after it), then `T* p = m_pObject; m_pObject = 0; return p;`.
extern char* g_pchNil;                                   // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;                    // 0x0143BB2C
class CString {                                          // libpk's MFC-style string (one word)
public:
    CString() : m_pchData(g_pchNil) { }
    ~CString();                                          // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);    // 0x00C0DEC0
    CString& __fastcall operator<<(long value);          // 0x00C0DF00
    char* m_pchData;
};
struct CDebugBreak {
    CDebugBreak();                                       // 0x00C16750
};
template <class T> class PKCAutoDelete {
public:
    T* Release()
    {
        if (m_pObject == 0) {
            {
                CString line;
                line << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 42L << ") : " << "Shouls have checked first..." << "\n";
                if (g_pfnAssertHook)
                    g_pfnAssertHook(line.m_pchData);
            }
            CDebugBreak();
        }
        T* object = m_pObject;
        m_pObject = 0;
        return object;
    }
    T* m_pObject;                                        // +0x00
};
class CUnprovenPKObject_00c099f0;
template class PKCAutoDelete<CUnprovenPKObject_00c099f0>;