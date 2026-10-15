// CWideString::operator=(wchar_t character) -- make the wide string hold the single character `character` (empty when it is L'\0'): release the current block, then allocate a fresh one for the one-character text
// Leaf: ??4CWideString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099B860, length 107 bytes (ret 4). Callers 0x005AE4F2, 0x0099C09D.
// Label correction: the manifest's "CCharString::UnassignString" is propagated; the body builds a two-character wide
// text from its 16-bit argument, inlines CWideString::UnassignString (0x0099B4D0) and calls
// CWideString::AllocStringData (0x0099B3C0), returning *this.
// Callees: free @ 0x00BFEA14 (the block's character buffer), operator delete @ 0x00BFE9BC (the 16-byte block),
// CWideString::AllocStringData(const wchar_t*, long) @ 0x0099B3C0.
// Layout: CWideStringData {basic_string<wchar_t> start +0, finish +4, end +8; NoRefs +0xc}.
// Shape: the {character, 0} text array is homed in the argument's own slot (`mov [esp+8],ax ; mov word ptr
// [esp+0Ah],0`); the release is the landed literal constructor's (0x0099B6B0) inline unassign: `--NoRefs <= 0`
// through a register copy, the block re-read and null-tested, its buffer freed when present, the block deleted,
// the handle cleared; the emptiness test reads the array's first element.
struct CWideStringData {                      // 16 bytes
    unsigned short* Start;                    // +0x00 basic_string<wchar_t>::_M_start
    unsigned short* Finish;                   // +0x04
    unsigned short* EndOfStorage;             // +0x08
    long            NoRefs;                   // +0x0c
};
extern "C" void __cdecl free(void* block);    // 0x00BFEA14
void __cdecl operator delete(void* block);    // 0x00BFE9BC

class CWideString {
public:
    CWideStringData* PStringData;             // +0x00
    CWideStringData* AllocStringData(const unsigned short* text, long length);   // 0x0099B3C0
    void UnassignString()                     // 0x0099B4D0, inlined
    {
        CWideStringData* data = PStringData;
        if (data != 0) {
            if (--data->NoRefs <= 0) {
                CWideStringData* block = PStringData;
                if (block != 0) {
                    if (block->Start != 0)
                        free(block->Start);
                    operator delete(block);
                }
            }
            PStringData = 0;
        }
    }
    const CWideString& operator=(unsigned short character);
};

const CWideString& CWideString::operator=(unsigned short character)
{
    unsigned short text[2];
    text[0] = character;
    text[1] = 0;
    UnassignString();
    if (text[0] != 0)
        PStringData = AllocStringData(text, -1);
    return *this;
}