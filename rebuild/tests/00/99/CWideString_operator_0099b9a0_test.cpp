#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
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

class CWideStringData { public:
    CWideBasicString Data;   // +0x00
    long NoRefs;             // +0x0C
};

class CWideString { public:
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

// ---- test stubs ----
static int g_fail = 0, g_own = 0, g_reserve = 0, g_lenerr = 0, g_alloc = 0, g_fillcalls = 0;
static size_t g_lastReserve = 0;
static jmp_buf g_jb;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }

void CWideString::MakeOwnCopyOfData() { ++g_own; }
void CWideBasicString::_M_throw_length_error() const { ++g_lenerr; longjmp(g_jb, 1); }
void CWideBasicString::reserve(size_t n)
{
    ++g_reserve; g_lastReserve = n;
    size_t sz = size();
    wchar_t* p = (wchar_t*)malloc((n + 1) * sizeof(wchar_t));
    memcpy(p, _M_start, (sz + 1) * sizeof(wchar_t));
    free(_M_start);
    _M_start = p; _M_finish = p + sz; _M_end_of_storage = p + n + 1;
}
wchar_t* __fastcall CWideBasicString::UninitFillN(wchar_t* first, size_t n, const wchar_t& value)
{
    ++g_fillcalls;
    for (size_t i = 0; i < n; ++i) first[i] = value;
    return first + n;
}
CWideStringData* CWideString::AllocStringData(const wchar_t* text, long length)
{
    ++g_alloc;
    check(length == -1, "alloc length");
    size_t len = 0; while (text[len]) ++len;
    CWideStringData* d = (CWideStringData*)malloc(sizeof(CWideStringData));
    d->Data._M_start = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    memcpy(d->Data._M_start, text, (len + 1) * sizeof(wchar_t));
    d->Data._M_finish = d->Data._M_start + len;
    d->Data._M_end_of_storage = d->Data._M_finish + 1;
    d->NoRefs = 1;
    return d;
}

int main()
{
    char buf[128];
    // 1. empty string, c == 0: nothing allocated
    CWideString s; s.PStringData = 0;
    const CWideString* r = &(s += (wchar_t)0);
    check(r == &s && s.PStringData == 0 && g_alloc == 0, "null + 0");
    // 2. empty string, c != 0: created from {c, 0}
    s += (wchar_t)'A';
    check(g_alloc == 1 && s.PStringData != 0 && s.PStringData->Data.size() == 1 && s.PStringData->Data._M_start[0] == 'A'
          && s.PStringData->Data._M_start[1] == 0 && g_own == 0, "null + A");
    // 3. append with no room: copy-on-write, grow to size + max(size, 1) = 2
    s += (wchar_t)'B';
    check(g_own == 1 && g_reserve == 1 && g_lastReserve == 2, "grow 1");
    check(s.PStringData->Data.size() == 2 && s.PStringData->Data._M_start[1] == 'B' && s.PStringData->Data._M_start[2] == 0, "append B");
    check(g_fillcalls == 1, "fill called");
    // 4. grow 2 -> 4, then an append that fits (no reserve)
    s += (wchar_t)'C';
    check(g_reserve == 2 && g_lastReserve == 4, "grow 2");
    s += (wchar_t)'D';
    check(g_reserve == 2 && g_own == 3, "no grow");
    check(memcmp(s.PStringData->Data._M_start, L"ABCD", 5 * sizeof(wchar_t)) == 0, "content ABCD");
    // 5. append of 0 to a live string still appends the character
    s.PStringData->Data.reserve(10);
    s += (wchar_t)0;
    check(s.PStringData->Data.size() == 5 && s.PStringData->Data._M_start[4] == 0, "append 0");
    // 6. length error: size == max_size()
    CWideStringData big; static wchar_t one[2];
    big.Data._M_start = one; big.Data._M_finish = (wchar_t*)((char*)one + 0xfffffffcUL); big.Data._M_end_of_storage = big.Data._M_finish; big.NoRefs = 1;
    CWideString t; t.PStringData = &big;
    if (setjmp(g_jb) == 0) { t += (wchar_t)'x'; check(0, "no length error"); }
    check(g_lenerr == 1, "length error");
    _snprintf(buf, sizeof(buf), "own=%d reserve=%d alloc=%d lenerr=%d", g_own, g_reserve, g_alloc, g_lenerr);
    check(strcmp(buf, "own=5 reserve=3 alloc=1 lenerr=1") == 0, buf);
    if (g_fail) return 1;
    puts("0099b9a0_TEST PASS");
    return 0;
}