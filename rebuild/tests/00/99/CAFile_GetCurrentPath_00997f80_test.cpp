// Behaviour test for 0x00997F80 (source body copied from src/00997f80.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
#include <stddef.h>
typedef unsigned short wchar16;
#define WEMPTY ((const wchar16*)L"")   // test only: g++ has a 4-byte wchar_t; every literal here is empty          // wchar_t is not a built-in type under the retail flags

extern "C" int __cdecl wcscmp(const wchar16* a, const wchar16* b);   // 0x00BFE94C

struct char_traits_w {
    static unsigned long length(const wchar16* s)
    {
        const wchar16* p = s;
        while (*p != 0)
            ++p;
        return p - s;
    }
    static int __cdecl compare(const wchar16* a, const wchar16* b, unsigned long n);   // 0x00403C60
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
    if ((data == 0 && wcscmp(WEMPTY, WEMPTY) == 0) || (data != 0 && data->Data == WEMPTY))
        CurrentPath = GetWorkingDirectory();
    return CurrentPath;
}

// ---- test harness ----
// Stubs: the CWideString members count copies and references; GetWorkingDirectory hands out a fixed data block.
static int g_cwd = 0, g_copies = 0, g_compares = 0;
static wchar16 g_text[4] = { 'a', 'b', 'c', 0 };
static CWideStringData g_cwdData;
static CWideStringData* g_cwdPtr = &g_cwdData;
static CWideStringData* g_nullPtr = 0;
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) { if (PStringData) ++PStringData->NoRefs; ++g_copies; }
CWideString::~CWideString() { if (PStringData) --PStringData->NoRefs; }
const CWideString& CWideString::operator=(const CWideString& other) { PStringData = other.PStringData; if (PStringData) ++PStringData->NoRefs; return *this; }
int __cdecl char_traits_w::compare(const wchar16* a, const wchar16* b, unsigned long n)
{
    ++g_compares;
    for (unsigned long i = 0; i < n; ++i) if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}
CWideString __fastcall CAFile::GetWorkingDirectory() { ++g_cwd; return *(const CWideString*)&g_cwdPtr; }
CWideString CAFile::CurrentPath(*(const CWideString*)&g_nullPtr);
int main()
{
    int ok = 1;
    g_cwdData.Data._M_start = g_text; g_cwdData.Data._M_finish = g_text + 3; g_cwdData.Data._M_end_of_storage = g_text + 4;
    g_cwdData.NoRefs = 1;
    // 1: no data block -> wcscmp(WEMPTY, WEMPTY) == 0 -> filled from the working directory
    {
        CWideString r = CAFile::GetCurrentPath();
        if (g_cwd != 1 || CAFile::CurrentPath.PStringData != &g_cwdData || r.PStringData != &g_cwdData) ok = 0;
    }
    // 2: non-empty path -> returned as is, no refill, no compare (size test fails first)
    {
        CWideString r = CAFile::GetCurrentPath();
        if (g_cwd != 1 || g_compares != 0 || r.PStringData != &g_cwdData) ok = 0;
    }
    // 3: empty data block -> the zero-length compare runs and the path is refilled
    g_cwdData.Data._M_finish = g_text;
    {
        CWideString r = CAFile::GetCurrentPath();
        if (g_cwd != 2 || g_compares != 1 || r.PStringData != &g_cwdData) ok = 0;
    }
    printf("00997f80_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}