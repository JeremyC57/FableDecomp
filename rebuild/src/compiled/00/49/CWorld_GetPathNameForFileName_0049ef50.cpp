// CWorld::GetPathNameForFileName(const CWideString& fileName) const -- search every top-level directory of U:\ for a file whose name (without extension, and with an empty extension) equals fileName; return the found entry, else the empty string
// Leaf: ?GetPathNameForFileName@CWorld@@QBE?AVCWideString@@ABV2@@Z
// Flags: /GS (no /Zc:wchar_t: retail's wchar_t is the <stddef.h> unsigned short typedef, owner 2026-10-29)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049EF50, 553 bytes, `ret 8`; /GS cookie frame (`and esp,-8`); FableWin.pdb and Ego_r.pdb
//   ?GetPathNameForFileName@CWorld@@QBE?AVCWideString@@ABV2@@Z (the PDB's `_W` spellings land in the G form).
// Callees: CWideString(const wchar_t*) 0x0099B6B0, CWideString() 0x0099AED0, CWideString(const CWideString&) 0x0099B720,
//   ~CWideString 0x0099B510, operator+(const CWideString&, const wchar_t*) 0x0099BF30 (__fastcall), the static __fastcall
//   CAFile::StartFileScan 0x00999760 / ScanNextFile 0x00999850 (wchar_t* overloads) / IsPathFile 0x00999A30 /
//   SplitPath 0x00997620, wcscmp 0x00BFE94C, STLport __char_traits_base<wchar_t, wint_t>::compare 0x00403C60 (cdecl,
//   out of line: it holds a loop); the empty CWideString global 0x013BCA24. Literals L"U:\\*", L"\\*.*", L"".
// Shape: both name equalities are the inline STLport 4.6 basic_string operator== (length() of the L"" literal is the
//   inlined __char_traits_base loop, which folds to 0, so the size test is `(finish - start) & ~1` and compare gets
//   `push 0`). Writing extension == L"" through the inline CWideString::operator==(const wchar_t*) (null case as
//   `if (wcscmp(..) == 0) return true; return false;`) is what fixed the old frame-slot residue: the hand-expanded
//   `continue` form of the parked source gave VC7.1 another flow graph and the six CWideString locals got the reverse
//   slot order. The CWideString == CWideString compare reads both data blocks into locals first (retail's
//   x = baseName in eax/edx, y = fileName in ecx/esi register choice).
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);   // 0x00BFE94C
extern "C" unsigned int __cdecl wcslen(const wchar_t* s);
#pragma intrinsic(wcslen)
namespace std {
template <class _CharT, class _IntT> class __char_traits_base { public:
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60 (wchar_t)
    static unsigned int __cdecl length(const _CharT* __s)
    {
        const _CharT _NullChar = _CharT();
        unsigned int __i;
        for (__i = 0; !(__s[__i] == _NullChar); ++__i) { }
        return __i;
    }
};
template <class _CharT> class char_traits : public __char_traits_base<_CharT, _CharT> { };
template <> class char_traits<wchar_t> : public __char_traits_base<wchar_t, unsigned short> { };
template <class _Tp> class allocator { };
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string { public:                            // STLport 4.6
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const _CharT* data() const { return _M_start; }
};
template <class _CharT, class _Traits, class _Alloc>
inline bool __cdecl operator==(const basic_string<_CharT, _Traits, _Alloc>& __x, const basic_string<_CharT, _Traits, _Alloc>& __y)
{
    return __x.size() == __y.size() && _Traits::compare(__x.data(), __y.data(), __x.size()) == 0;
}
template <class _CharT, class _Traits, class _Alloc>
inline bool __cdecl operator==(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __t)
{
    unsigned int __n = _Traits::length(__t);
    return __s.size() == __n && _Traits::compare(__s.data(), __t, __n) == 0;
}
}
class CWideStringData { public:
    std::basic_string<wchar_t> Data;                    // +0x00
    long NoRefs;                                        // +0x0C
};
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    __forceinline bool operator==(const wchar_t* text) const
    {
        if (PStringData == 0) {
            if (wcscmp(text, L"") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    __forceinline bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        const CWideStringData* a = PStringData; const CWideStringData* b = other.PStringData;
        return a->Data == b->Data;
    }
};
extern const CWideString g_EmptyWideString;             // 0x013BCA24
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30
class CAFile { public:
    class CFileScanInfo { public:
        bool Stopped;                                   // +0x00
        long SearchHandle;                              // +0x04
    };
    static bool __fastcall StartFileScan(const CWideString& pattern, CFileScanInfo& info, wchar_t* firstFile);   // 0x00999760
    static bool __fastcall ScanNextFile(const CWideString& pattern, CFileScanInfo& info, wchar_t* nextFile);     // 0x00999850
    static bool __fastcall IsPathFile(const CWideString& path);                                                   // 0x00999A30
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* fname, CWideString* ext);   // 0x00997620
};

class CWorld : public CWorldLayout { public:
    CWideString GetPathNameForFileName(const CWideString& fileName) const;
};

CWideString CWorld::GetPathNameForFileName(const CWideString& fileName) const
{
    CWideString pattern(L"U:\\*");
    CAFile::CFileScanInfo info;
    wchar_t dirName[262];
    if (CAFile::StartFileScan(pattern, info, dirName)) {
        do {
            CWideString dir(dirName);
            if (!CAFile::IsPathFile(dir)) {
                CWideString subPattern = dir + L"\\*.*";
                CAFile::CFileScanInfo subInfo;
                wchar_t name[262];
                if (CAFile::StartFileScan(subPattern, subInfo, name)) {
                    do {
                        CWideString path(name);
                        CWideString baseName;
                        CWideString extension;
                        CAFile::SplitPath(path, 0, 0, &baseName, &extension);
                        if (extension == L"" && baseName == fileName)
                            return CWideString(name);
                    } while (CAFile::ScanNextFile(subPattern, subInfo, name));
                }
            }
        } while (CAFile::ScanNextFile(pattern, info, dirName));
    }
    return g_EmptyWideString;
}