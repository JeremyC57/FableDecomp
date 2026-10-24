// CWideString::SetAsSubString(const wchar_t* text, long length) -- drop the current block and take `length` characters of text (all of it for -1); empty text leaves the string empty
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099B770, 93 bytes. FableWin.pdb ?SetAsSubString@CWideString@@QAEXPB_WJ@Z:
// `__fastcall`, `ret 8`. Manifest label "CCharString_Replace" is propagated (GOTCHAS chunk 178 names it).
// Callees: free @ 0x00BFEA14 (the STLport wstring buffer), operator delete @ 0x00BFE9BC (the 16-byte block),
// CWideString::AllocStringData(const wchar_t*, long) @ 0x0099B3C0.
// Shape: UnassignString is expanded inline (the out-of-line copy is 0x0099B4D0): NoRefs-- through a LOCAL copy of the
// block pointer (the member form emits a memory dec), then the member is re-read for `delete PStringData` (its inline ~basic_string frees the buffer) and zeroed;
// then the landed CWideString(const wchar_t*) assignment tail (null/empty test, AllocStringData).
// Layout: CWideStringData = STLport basic_string<wchar_t> {_M_start, _M_finish, _M_end_of_storage} + NoRefs +0xC.
// Leaf: ?SetAsSubString@CWideString
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

struct CBasicStringW {                                  // STLport basic_string<wchar_t>, 12 bytes
    wchar16* _M_start;                                  // +0x00
    wchar16* _M_finish;                                 // +0x04
    wchar16* _M_end_of_storage;                         // +0x08
    ~CBasicStringW()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};
struct CWideStringData {                                // 16 bytes
    CBasicStringW Data;                                 // +0x00
    long          NoRefs;                               // +0x0c
};

class CWideString
{
public:
    CWideStringData* PStringData;                       // +0x00
    void UnassignString()                               // out-of-line copy 0x0099B4D0
    {
        CWideStringData* data = PStringData;
        if (data != 0) {
            if (--data->NoRefs <= 0)
                delete PStringData;
            PStringData = 0;
        }
    }
    CWideStringData* AllocStringData(const wchar16* text, long length);   // 0x0099B3C0
    void SetAsSubString(const wchar16* text, long length);
};

void CWideString::SetAsSubString(const wchar16* text, long length)
{
    UnassignString();
    if (text != 0 && *text != 0)
        PStringData = AllocStringData(text, length);
}