// CString::operator=(const CString& stringSrc) -- MFC-style assignment: when either string is locked (negative reference count, the nil
// header excepted) copy the characters through AssignCopy, else release ours and share the source's data (InterlockedIncrement)
// Leaf: ??4CString@@QAEABV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0C0C0, __thiscall `ret 4`. /O2 /Oy.
// Naming (naming wave 24, DECISIONS_W23_RESOLVED item 9): the libpk CString's operator=(const CString&), the spelling its caller
//   0x00C2E960 binds and the landed AssignCopy 0x00C0C060 / operator=(const char*) 0x00C0C200 rows name (MFC strcore.cpp's
//   CString::operator=); was landed as a propagated CCharString::Assign. The nil header 0x0139C8FC is _NullString: retail's own assert
//   text "stringSrc.GetData() != _NullString" names it and the landed CString(const CString&) 0x00C0DAB0 binds that spelling; the assert
//   hook 0x0143BB2C is g_pfnAssertHook (the landed ~CString 0x00C0BA40).
// Callees: CString::AssignCopy 0x00C0C060, CString::Release 0x00C0B910, InterlockedIncrement, CDebugBreak::CDebugBreak (in this TU, as
//   the row landed). Literals "m_pchData != NULL\n", "stringSrc.GetData() != _NullString\n".
// Layout: the object is one char pointer; the 12-byte CStringData header (nRefs, nDataLength, nAllocLength) precedes the characters.
// Flags: /O2 /Oy
#include "fable_compiler.h"
#include <stddef.h>
struct CStringData { long nRefs; int nDataLength, nAllocLength; };
extern CStringData* _NullString;                    // 0x0139C8FC
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;               // 0x0143BB2C
struct CDebugBreak { CDebugBreak(); };
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#pragma auto_inline(off)
CDebugBreak::CDebugBreak() { DebugBreak(); }
#pragma auto_inline(on)
#define LIB_ASSERT(cond) \
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\n"); CDebugBreak token; }
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long*);
class CString {
public:
    char* m_pchData;
    FABLE_FORCEINLINE CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    void Release();                                 // 0x00C0B910
    void AssignCopy(int nSrcLen, const char* lpszSrcData);   // 0x00C0C060
    const CString& operator=(const CString& stringSrc);
};

const CString& CString::operator=(const CString& stringSrc)
{
    if (m_pchData != stringSrc.m_pchData) {
        if ((GetData()->nRefs < 0 && GetData() != _NullString) || stringSrc.GetData()->nRefs < 0) {
            AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
        } else {
            Release();
            LIB_ASSERT(stringSrc.GetData() != _NullString)
            m_pchData = stringSrc.m_pchData;
            InterlockedIncrement(&GetData()->nRefs);
        }
    }
    return *this;
}