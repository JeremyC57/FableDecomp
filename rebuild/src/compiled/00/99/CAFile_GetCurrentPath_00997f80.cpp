// CAFile::GetCurrentPath() -- the cached current path; on first use (or while it is empty) it is filled from the process working directory
// Retail: Fable.exe (TLC, VC7.1) @ 0x00997F80, 132 bytes. FableWin.pdb ?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ:
// a static `__fastcall` returning CWideString by value (hidden result pointer in ecx), plain `ret`.
// Label: the manifest's "CInputProcessMain_GetCameraPath" is propagated; the landed callers (PathExists 0x00999230,
// ScanNextFile 0x00999850) name it GetCurrentPath / GetCurrentWorkingDirectory.
// Callees: wcscmp @ 0x00BFE94C, __char_traits_base<wchar_t, wint_t>::compare @ 0x00403C60 (cdecl, inherited by char_traits<wchar_t>; manifest "MemCmp_Unsigned16"),
// the working-directory reader @ 0x009974A0 (static `__fastcall`, `_wgetcwd` into a MAX_PATH buffer -> CWideString;
// descriptive name GetWorkingDirectory), CWideString::operator=(const CWideString&) @ 0x0099B7D0, ~CWideString @
// 0x0099B510, CWideString(const CWideString&) @ 0x0099B720.
// Globals: CAFile::CurrentPath (CWideString) @ 0x013BCA10; literal L"" @ 0x0122D70C.
// Shape: one `return CurrentPath;` after `if ((data == 0 && wcscmp(L"", L"") == 0) || (data != 0 && data->Data == L""))
// CurrentPath = GetWorkingDirectory();` -- VC7.1 threads the second null test, tail-duplicates the return copy into the
// null path and pushes &CurrentPath before `mov ecx,esi` in both copies (two source-level `return CurrentPath;`
// statements emit `mov ecx,esi` first). The STLport `wstring == const wchar_t*` is forced inline (traits::length of the
// literal folds to 0, so the size test is `(finish - start) & ~1`, then the out-of-line traits compare over 0 chars).
// Grade: RELOCATION_MATCH (agent902, 5th grade) -- a single tail-duplicated return; the goto form with two returns scheduled `mov ecx,esi` before the push (132v132).
// Leaf: ?GetCurrentPath@CAFile
#include <stddef.h>
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern "C" int __cdecl wcscmp(const wchar16* a, const wchar16* b);   // 0x00BFE94C

namespace std {
template <class _CharT, class _IntT> class __char_traits_base { public:   // STLport 4.6 char_traits.h; wint_t == unsigned short
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60
};
}
struct char_traits_w : public std::__char_traits_base<wchar16, wchar16> {   // STLport char_traits<wchar_t>
    static unsigned long length(const wchar16* s)
    {
        const wchar16* p = s;
        while (*p != 0)
            ++p;
        return p - s;
    }
};

struct CBasicStringW {                                  // STLport basic_string<wchar_t>, 12 bytes
    wchar16* _M_start;                                  // +0x00
    wchar16* _M_finish;                                 // +0x04
    wchar16* _M_end_of_storage;                         // +0x08
    unsigned long size() const { return _M_finish - _M_start; }
    const wchar16* data() const { return _M_start; }
};
__forceinline bool operator==(const CBasicStringW& s, const wchar16* text)
{
    unsigned long n = char_traits_w::length(text);
    return s.size() == n && char_traits_w::compare(s.data(), text, n) == 0;
}

class CWideStringData { public:                                // 16 bytes
    CBasicStringW Data;                                 // +0x00
    long          NoRefs;                               // +0x0c
};

class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

class CAFile
{
public:
    static CWideString CurrentPath;                                                             // 0x013BCA10
    static CWideString __fastcall GetWorkingDirectory();                                        // 0x009974A0
    static CWideString __fastcall GetCurrentPath();
};

CWideString __fastcall CAFile::GetCurrentPath()
{
    CWideStringData* data = CurrentPath.PStringData;
    if ((data == 0 && wcscmp(L"", L"") == 0) || (data != 0 && data->Data == L""))
        CurrentPath = GetWorkingDirectory();
    return CurrentPath;
}