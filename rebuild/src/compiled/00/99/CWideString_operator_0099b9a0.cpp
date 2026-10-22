// CWideString::operator+=(wchar_t c) -- append one wide character: copy-on-write then the inline STLport append(1, c) (length check, grow to size + max(size, 1), fill, terminate); an empty string is created from {c, 0} unless c is 0
// Leaf: ??YCWideString@@QAEABU0@G@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099B9A0, 208 bytes incl. padding (ret 4). Manifest "CCharString::InsertChar" is
//          propagated: the body is CWideString's (sibling of the landed operator+=(const CWideString&) 0x0099B8D0).
// Callees: CWideString::MakeOwnCopyOfData @ 0x0099B560, basic_string<wchar_t>::_M_throw_length_error @ 0x0099C550,
//          basic_string<wchar_t>::reserve @ 0x0099C990, uninitialized_fill_n<unsigned short> @ 0x005123A0 (__fastcall:
//          first in ecx, count in edx, &value pushed), CWideString::AllocStringData(const wchar_t*, long) @ 0x0099B3C0.
// Layout: CWideStringData {basic_string<wchar_t> {_M_start, _M_finish, _M_end_of_storage}, NoRefs +0xC}.
// Shape: append copies its character first and its count second (the count lands in the caller's dead character slot,
//          the copy in the frame); the length error is noreturn, so its call is laid out of line.
#include <stddef.h>

template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

struct CWideBasicString {
    wchar_t* _M_start;            // +0x00
    wchar_t* _M_finish;           // +0x04
    wchar_t* _M_end_of_storage;   // +0x08
    size_t size() const { return _M_finish - _M_start; }
    size_t max_size() const { return 0x7ffffffe; }
    size_t capacity() const { return (_M_end_of_storage - _M_start) - 1; }
    __declspec(noreturn) void _M_throw_length_error() const;   // 0x0099C550
    void reserve(size_t n);                        // 0x0099C990
    __forceinline CWideBasicString& append(size_t n, wchar_t c)
    {
        wchar_t fill = c;
        size_t count = n;
        if (count > max_size() || size() > max_size() - count)
            _M_throw_length_error();
        if (size() + count > capacity()) {
            size_t sz = size();
            reserve(sz + Max(sz, count));
        }
        UninitFillN(_M_finish + 1, count - 1, fill);
        _M_finish[count] = 0;
        *_M_finish = c;
        _M_finish += count;
        return *this;
    }
    static wchar_t* __fastcall UninitFillN(wchar_t* first, size_t n, const wchar_t& value);   // 0x005123A0
};

struct CWideStringData {
    CWideBasicString Data;   // +0x00
    long NoRefs;             // +0x0C
};

struct CWideString {
    CWideStringData* PStringData;   // +0x00
    void MakeOwnCopyOfData();       // 0x0099B560
    CWideStringData* AllocStringData(const wchar_t* text, long length);   // 0x0099B3C0
    const CWideString& operator+=(wchar_t c);
};

const CWideString& CWideString::operator+=(wchar_t c)
{
    if (PStringData != 0) {
        MakeOwnCopyOfData();
        PStringData->Data.append(1, c);
    } else {
        wchar_t text[2];
        text[0] = c;
        text[1] = 0;
        if (c != 0)
            PStringData = AllocStringData(text, -1);
    }
    return *this;
}