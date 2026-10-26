// CWideString::FindCharPos(long c) -- index of the first occurrence of c, or -1. The text pointer is an
// inline accessor (block ? block->Start : the empty wide literal) evaluated twice: once for wcschr and
// once to form the difference (pointer subtraction on wchar_t*, hence the `sar eax,1`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099B070, length 72 bytes.
// Callees: wcschr @ 0x00BFED8E (cdecl). Globals: empty wide string @ 0x0129A8E0.
// Manifest label (CCharString::FindCharPos) corrected: the halved difference and wcschr are the wide class.
// The character argument is read as a dword (no movzx): a `long`, truncated by wcschr's wchar_t parameter.
// CWideString model: a 4-byte handle on a shared 16-byte block whose first three dwords are the retail
// (STLport-era) basic_string<wchar_t> {Start, Finish, EndOfStorage} and the fourth is NoRefs.
// (The Ego_r PDB shows a 28-byte VC7 basic_string there; retail is the 12-byte form.)
#include <stddef.h>
class CWideStringData { public:
    wchar_t* Start;                             // +0x00
    wchar_t* Finish;                            // +0x04
    wchar_t* EndOfStorage;                      // +0x08
    long     NoRefs;                            // +0x0c
};
extern "C" wchar_t* __cdecl wcschr(const wchar_t* text, wchar_t c);   // 0x00BFED8E
extern wchar_t g_emptyWideString[1];                                 // 0x0129A8E0

class CWideString {
public:
    CWideStringData* PStringData;               // +0x00
    const wchar_t* GetText() const { return PStringData == 0 ? g_emptyWideString : PStringData->Start; }
    long FindCharPos(long c) const;
};

long CWideString::FindCharPos(long c) const
{
    const wchar_t* found = wcschr(GetText(), (wchar_t)c);
    if (found != 0)
        return found - GetText();
    return -1;
}