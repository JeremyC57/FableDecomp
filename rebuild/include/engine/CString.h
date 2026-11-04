#ifndef FABLE_PASS11_CSTRING_H
#define FABLE_PASS11_CSTRING_H
#include <stddef.h>
#include <stdarg.h>
// Retail opt-in (Codex pass 11): no pre-existing engine/CString.h was supplied.
// Layout: real libpk MFC-derived CString has its sole character pointer at +0;
// CStringData is the preceding three native long words at -12, -8 and -4.
// Native SetAt/Replace/FormatV and original MFC CString declarations corroborate.
// Retail opt-in (chunk 750, row 0x00C21040 CSoundEvent::Describe): define FABLE_RETAIL_CSTRING_BUILDERS for the out-of-line
// constructors, destructor and the `__fastcall` stream builders the libpk trace code chains (`mov edx,arg ; mov ecx,eax ; call`;
// the double builder takes its argument on the stack, `fld qword [esp+4]` ... `ret 8`). The spellings are the ones bound to
// these addresses in rebuild/oracles/reloc_bindings.tsv (operator<<(double) 0x00C0DED0 was unbound; same family). Declarations
// only: no layout change.
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;
struct CDebugBreak { CDebugBreak(); };
#define IsValidString(lpsz) ((lpsz) != NULL)
#define LIB_ASSERT(cond) if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak(); }
struct CStringData {
    long nRefs;
    long nDataLength;
    long nAllocLength;
    char* data() { return (char*)(this + 1); }
};
class CString {
public:
    char* m_pchData;
    CStringData* GetData() const {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData)-1;
    }
    int GetAllocLength() const { return GetData()->nAllocLength; }
    char* GetBuffer(int nMinBufLength);
    void ReleaseBuffer(int nNewLength = -1);
    void FormatV(const char* lpszFormat, va_list argList);
#ifdef FABLE_RETAIL_CSTRING_BUILDERS
    CString(const char* text);                               // 0x00C0BFE0 ??0CString@@QAE@PBD@Z
    CString(const CString& other);                           // 0x00C0DAB0 ??0CString@@QAE@ABV0@@Z
    ~CString();                                              // 0x00C0BA40 ??1CString@@QAE@XZ
    CString& __fastcall operator<<(const char* text);        // 0x00C0DEC0 ??6CString@@QAIAAV0@PBD@Z
    CString& __fastcall operator<<(unsigned long value);     // 0x00C0DF10 ??6CString@@QAIAAV0@K@Z
    CString& __fastcall operator<<(long value);              // 0x00C0DF00 ??6CString@@QAIAAV0@J@Z
    CString& __fastcall operator<<(double value);            // 0x00C0DED0 ??6CString@@QAIAAV0@N@Z
#endif
#ifdef FABLE_RETAIL_CSTRING_ALLOCCOPY
    // Chunk 767 (row 0x00C0BF90): MFC strcore.cpp AllocCopy / AllocBuffer / Init, the spellings Mid 0x00C0DF50, Right 0x00C0E0F0 and
    // Left 0x00C0E1F0 bind (?AllocCopy@CString@@QBEXAAV1@HHH@Z, ?AllocBuffer@CString@@QAEXH@Z 0x00C0BDA0). Init() is inline in retail:
    // `m_pchData = _afxPchNil` (the nil characters 0x0139C900; the spelling 40 landed rows bind, naming decision D4 still HOLD).
    void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;   // 0x00C0BF90
    void AllocBuffer(int nLen);                              // 0x00C0BDA0
    void Init();
#endif
};
#ifdef FABLE_RETAIL_CSTRING_ALLOCCOPY
extern char* _afxPchNil;                                     // 0x0139C900
inline void CString::Init() { m_pchData = _afxPchNil; }
#endif
#endif
