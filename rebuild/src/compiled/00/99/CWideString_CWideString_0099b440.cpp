// CWideString::CWideString(const CCharString&) -- widen a narrow string (byte -> wchar_t, Latin-1)
// Leaf: ??0CWideString@@QAE@ABVCCharString
// Retail: Fable.exe @ 0x0099B440, 135 bytes (+int3). PDB ??0CWideString@@QAE@ABVCCharString@@@Z (explicit)
// Callees: operator new 0x00BFEA1A, STLport basic_string<wchar_t>(size_type n, wchar_t c, const allocator&)
// 0x0099CA70 (label "WideString_Allocate"). Global: CWideString instance counter 0x013BCA20 (as in the landed
// 0x0099B6B0 constructor), the CCharString empty text "" 0x0129AAF4.
// Retail CWideStringData (16 bytes): +0x00 basic_string<wchar_t> {_M_start, _M_finish, _M_end_of_storage},
// +0x0C NoRefs. CCharStringData: +0x00 char* text, +0x04 long length.
// Shape: the counter is bumped first; the data is new'd as a string of `length` spaces with NoRefs = 1 (inline
// CWideStringData constructor; the default allocator temporary lives in the dead parameter slot); then every
// byte is stored through the re-read PStringData->_M_start, the source text re-fetched through the null-safe
// inline accessor each step; the conversion is `c & 0xFF` on a plain (signed) char (movsx + and).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

void* __cdecl operator new(size_t size);   // 0x00BFEA1A

extern long g_CWideStringInstanceCount;    // 0x013BCA20
extern char g_CCharStringEmptyText[];      // 0x0129AAF4 ("")

struct CWideAllocator { CWideAllocator() {} };

struct CWideBasicString {
    wchar_t* _M_start;            // +0x00
    wchar_t* _M_finish;           // +0x04
    wchar_t* _M_end_of_storage;   // +0x08
    CWideBasicString(size_t n, wchar_t c, const CWideAllocator& a = CWideAllocator());   // 0x0099CA70
};

class CWideStringData { public:
    CWideBasicString Data;   // +0x00
    long NoRefs;             // +0x0C
    CWideStringData(long length, wchar_t fill) : Data(length, fill) { NoRefs = 1; }
};

class CCharStringData { public:
    char* Text;     // +0x00
    long Length;    // +0x04
};

class CCharString { public:
    CCharStringData* PStringData;   // +0x00
    long GetLength() const { if (PStringData == 0) return 0; return PStringData->Length; }
    const char* GetText() const { if (PStringData == 0) return g_CCharStringEmptyText; return PStringData->Text; }
};

class CWideString { public:
    CWideStringData* PStringData;   // +0x00
    explicit CWideString(const CCharString& text);
};

CWideString::CWideString(const CCharString& text)
{
    ++g_CWideStringInstanceCount;
    long length = text.GetLength();
    PStringData = new CWideStringData(length, L' ');
    for (long i = 0; i < length; ++i)
        PStringData->Data._M_start[i] = (wchar_t)(text.GetText()[i] & 0xFF);
}