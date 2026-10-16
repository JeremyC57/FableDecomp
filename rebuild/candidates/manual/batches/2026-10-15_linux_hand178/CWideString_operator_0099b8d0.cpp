// CWideString::operator+=(const CWideString&) -- append another wide string (copy-on-write, shares when empty)
// Leaf: ??YCWideString@@QAEABU0@ABU0@@Z
// Retail: Fable.exe @ 0x0099B8D0, 104 bytes (+int3). PDB ??YCWideString@@QAEABV0@ABV0@@Z
// Callees: CWideString::MakeOwnCopyOfData() 0x0099B560 (label says CCharString; the body works on CWideString),
// STLport basic_string<wchar_t>::_M_append(const wchar_t*, const wchar_t*, const forward_iterator_tag&)
// 0x0099C7E0 (label "CCharString_Insert"). Global: the shared empty wide text L"" 0x0129A8E0.
// Retail CWideStringData (16 bytes): +0x00 basic_string<wchar_t> {_M_start, _M_finish, _M_end_of_storage},
// +0x0C NoRefs.
// Shape: a non-empty string is unshared then appended with the other's text (null-safe inline accessor, inline
// append(s) -> append(s, s + traits::length(s)) -> _M_append whose iterator tag temporary sits in the dead
// parameter slot; the accessor is the null-first ternary);
// an empty string simply shares the other's data block (NoRefs + 1) unless it is itself or empty.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

extern wchar_t g_FableEmptyWideString[];   // 0x0129A8E0

struct forward_iterator_tag { forward_iterator_tag() {} };

static inline size_t WideLength(const wchar_t* s) { size_t n = 0; while (s[n] != 0) ++n; return n; }

struct CWideBasicString {
    wchar_t* _M_start;            // +0x00
    wchar_t* _M_finish;           // +0x04
    wchar_t* _M_end_of_storage;   // +0x08
    CWideBasicString& _M_append(const wchar_t* first, const wchar_t* last, const forward_iterator_tag& tag);   // 0x0099C7E0
    CWideBasicString& append(const wchar_t* first, const wchar_t* last)
    { return _M_append(first, last, forward_iterator_tag()); }
    CWideBasicString& append(const wchar_t* s) { return append(s, s + WideLength(s)); }
};

struct CWideStringData {
    CWideBasicString Data;   // +0x00
    long NoRefs;             // +0x0C
};

struct CWideString {
    CWideStringData* PStringData;   // +0x00
    void MakeOwnCopyOfData();       // 0x0099B560
    const wchar_t* GetText() const { return PStringData == 0 ? g_FableEmptyWideString : PStringData->Data._M_start; }
    const CWideString& operator+=(const CWideString& other);
};

const CWideString& CWideString::operator+=(const CWideString& other)
{
    if (PStringData != 0)
    {
        MakeOwnCopyOfData();
        PStringData->Data.append(other.GetText());
    }
    else if (&other != this && other.PStringData != 0)
    {
        PStringData = other.PStringData;
        ++PStringData->NoRefs;
    }
    return *this;
}
