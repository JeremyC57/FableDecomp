// Behaviour test for 0x0099bf30: the candidate source below is embedded verbatim, followed by stand-ins for its callees
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; printf("check failed line %d\n", __LINE__); } } while (0)
// ---- embedded candidate source ----
// operator+(const CWideString& left, const unsigned short* right) -- concatenate a CWideString and a wide literal into a new CWideString
// Leaf: ??H@YI?AVCWideString@@ABV0@PBG@Z
// Retail: Fable.exe 0x0099BF30, 177 bytes; FableWin.pdb ??H@YI?AVCWideString@@ABV0@PB_W@Z (wide text spelled `unsigned short`,
//   owner 2026-10-29): a free __fastcall returning CWideString by value (hidden result in ecx, left in edx, right on the
//   stack, `ret 4`). Callees: CWideString::operator+=(const unsigned short*) 0x0099B940, CWideString::UnassignString
//   0x0099B4D0, free 0x00BFEA14, operator delete 0x00BFE9BC. Re-authors the landed CWideString_AddWideStringAndLiteral asm.
// Shape: `CWideString result(left); result += right; return result;` -- VC7.1 has no NRVO, so the return copies through the
//   inline copy constructor (zero the block, count++, the inline operator= self/same-block tests, the out-of-line
//   UnassignString, share + NoRefs++) and then runs the local's inline destructor (NoRefs--, on zero free the wstring buffer
//   and delete the block, count--). The address-taking callees (CWideString.cpp) are same-TU noinline bodies so the local's
//   address never escapes and its block pointer stays in esi across UnassignString (model of the landed sibling
//   operator+(const unsigned short*, const CWideString&) 0x0099BFF0, which this file reuses verbatim).
// Globals: g_CWideStringInstanceCount 0x013BCA20 (+1 per constructor, -1 per destructor).
// Layout: CWideStringData = STLport basic_string<unsigned short> {_M_start, _M_finish, _M_end_of_storage} + NoRefs +0xC.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

extern long g_CWideStringInstanceCount;                 // 0x013BCA20
extern unsigned short g_EmptyWideString[];              // 0x0129A8E0

struct forward_iterator_tag {
    forward_iterator_tag() { }
};
struct CBasicStringW {                                  // STLport basic_string<wchar_t>, 12 bytes
    wchar16* _M_start;                                  // +0x00
    wchar16* _M_finish;                                 // +0x04
    wchar16* _M_end_of_storage;                         // +0x08
    static unsigned long length(const wchar16* s)
    {
        unsigned long n = 0;
        while (s[n] != 0)
            ++n;
        return n;
    }
    CBasicStringW(const wchar16* s, unsigned long n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_range_initialize(s, s + n, forward_iterator_tag());
    }
    ~CBasicStringW()
    {
        if (_M_start != 0)
            free(_M_start);
    }
    void _M_range_initialize(const wchar16* first, const wchar16* last, const forward_iterator_tag& tag);   // 0x0099C670
    CBasicStringW& _M_append(const wchar16* first, const wchar16* last, const forward_iterator_tag& tag);   // 0x0099C7E0
    CBasicStringW& append(const wchar16* s) { return _M_append(s, s + length(s), forward_iterator_tag()); }
};
class CWideStringData { public:                                // 16 bytes
    CBasicStringW Data;                                 // +0x00
    long          NoRefs;                               // +0x0c
    CWideStringData(const wchar16* text, unsigned long n) : Data(text, n), NoRefs(1) { }
};

class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    CWideString(const wchar16* text);                   // 0x0099B6B0
    CWideString(const CWideString& other) : PStringData(0)
    {
        ++g_CWideStringInstanceCount;
        *this = other;
    }
    ~CWideString()
    {
        if (PStringData != 0) {
            if (--PStringData->NoRefs <= 0)
                delete PStringData;
            PStringData = 0;
        }
        --g_CWideStringInstanceCount;
    }
    const CWideString& operator=(const CWideString& other)
    {
        if (&other != this) {
            if (other.PStringData != PStringData) {
                if (PStringData != 0)
                    UnassignString();
                if (other.PStringData != 0) {
                    PStringData = other.PStringData;
                    ++PStringData->NoRefs;
                }
            }
        }
        return *this;
    }
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    const CWideString& operator+=(const wchar16* text);         // 0x0099B940
    const wchar16* GetText() const { return PStringData == 0 ? g_EmptyWideString : PStringData->Data._M_start; }
    void UnassignString();                              // 0x0099B4D0
    void MakeOwnCopyOfData();                           // 0x0099B560
    CWideStringData* AllocStringData(const wchar16* text, long length);   // 0x0099B3C0
};

// ---- same-TU callee bodies (CWideString.cpp), defined before the graded function ----
__declspec(noinline) void CWideString::UnassignString()
{
    if (PStringData != 0) {
        if (--PStringData->NoRefs <= 0)
            delete PStringData;
        PStringData = 0;
    }
}

__declspec(noinline) CWideStringData* CWideString::AllocStringData(const wchar16* text, long length)
{
    return new CWideStringData(text, length == -1 ? CBasicStringW::length(text) : length);
}

__declspec(noinline) void CWideString::MakeOwnCopyOfData()
{
    CWideStringData* data = PStringData;
    if (data != 0 && data->NoRefs > 1) {
        CWideStringData* copy = AllocStringData(data->Data._M_start, -1);
        UnassignString();
        PStringData = copy;
    }
}

__declspec(noinline) CWideString::CWideString(const wchar16* text) : PStringData(0)
{
    ++g_CWideStringInstanceCount;
    if (text != 0 && *text != 0)
        PStringData = AllocStringData(text, -1);
}

__declspec(noinline) const CWideString& CWideString::operator+=(const CWideString& other)
{
    if (PStringData != 0) {
        MakeOwnCopyOfData();
        PStringData->Data.append(other.GetText());
    } else if (&other != this && other.PStringData != 0) {
        PStringData = other.PStringData;
        ++PStringData->NoRefs;
    }
    return *this;
}

__declspec(noinline) const CWideString& CWideString::operator+=(const wchar16* text)
{
    if (PStringData != 0) {
        MakeOwnCopyOfData();
        PStringData->Data.append(text);
    } else if (text != 0 && *text != 0) {
        PStringData = AllocStringData(text, -1);
    }
    return *this;
}

// ---- graded ----
CWideString __fastcall operator+(const CWideString& left, const wchar16* right)
{
    CWideString result(left);
    result += right;
    return result;
}
// ---- end of embedded source ----
// ---- stand-ins for the out-of-TU callees and globals ----
long g_CWideStringInstanceCount = 0;
unsigned short g_EmptyWideString[1] = { 0 };
void CBasicStringW::_M_range_initialize(const wchar16* first, const wchar16* last, const forward_iterator_tag&)
{
    unsigned long n = (unsigned long)(last - first);
    _M_start = (wchar16*)malloc((n + 1) * sizeof(wchar16));
    memcpy(_M_start, first, n * sizeof(wchar16));
    _M_start[n] = 0;
    _M_finish = _M_start + n;
    _M_end_of_storage = _M_finish + 1;
}
CBasicStringW& CBasicStringW::_M_append(const wchar16* first, const wchar16* last, const forward_iterator_tag&)
{
    unsigned long old = (unsigned long)(_M_finish - _M_start), n = (unsigned long)(last - first);
    wchar16* p = (wchar16*)malloc((old + n + 1) * sizeof(wchar16));
    if (old) memcpy(p, _M_start, old * sizeof(wchar16));
    memcpy(p + old, first, n * sizeof(wchar16));
    p[old + n] = 0;
    if (_M_start) free(_M_start);
    _M_start = p; _M_finish = p + old + n; _M_end_of_storage = _M_finish + 1;
    return *this;
}
static bool Eq(const CWideString& s, const char* text)
{
    const wchar16* w = s.GetText();
    unsigned long i = 0;
    for (; text[i]; ++i) if (w[i] != (wchar16)text[i]) return false;
    return w[i] == 0;
}
static wchar16* W(const char* text, wchar16* buffer)
{
    unsigned long i = 0;
    for (; text[i]; ++i) buffer[i] = (wchar16)text[i];
    buffer[i] = 0;
    return buffer;
}
int main()
{
    wchar16 b1[32], b2[32], b3[32];
    {
        CWideString hello(W("Hello, ", b1)), empty((const wchar16*)0);
        CWideString a = hello + W("world", b2);
        CHECK(Eq(a, "Hello, world"));
        CHECK(Eq(hello, "Hello, "));                       // the left operand keeps its own text
        CWideString b = empty + W("abc", b3);
        CHECK(Eq(b, "abc"));
        CWideString c = hello + W("", b3);                  // the shared copy is split off before the (empty) append
        CHECK(Eq(c, "Hello, ") && c.PStringData != hello.PStringData && hello.PStringData->NoRefs == 1);
        CWideString e = empty + (const wchar16*)0;          // empty + null stays empty
        CHECK(e.PStringData == 0);
        CWideString d = empty + W("", b3);
        CHECK(d.PStringData == 0);
    }
    CHECK(g_CWideStringInstanceCount == 0);
    if (g_ok) printf("0099bf30_TEST PASS\n");
    return g_ok ? 0 : 1;
}