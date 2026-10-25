// CWStringParser::ReadNextChunkAsString(CWParsedItem* item) -- read the run at the cursor as a string item: a whitespace run when the cursor is on whitespace, else a non-whitespace run
// Leaf: ?ReadNextChunkAsString@CWStringParser@@QAE_NPAVCWParsedItem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D0A0, 60 bytes (+4 int3 padding), thiscall, `ret 4`.
//   PDB name ?ReadNextChunkAsString@CWStringParser@@QAE_NPAVCWParsedItem@@@Z.
// Callees: iswspace 0x00BFEB90 (MSVCR71 import thunk), ReadAsStringUntilNonWhitespace 0x00A0CC50 and
//   ReadAsStringUntilWhitespace 0x00A0CD40 (both landed).
// Layout: Ego_r.pdb CWStringParser (vptr, String +0x04, StringPos +0x08, Line +0x0c).
// Shape: the wchar_t argument is zero-extended through `xor edx,edx ; mov dx,..`; both readers are returned straight
//   (`call ; pop esi ; ret 4` tails, no sibling jump because esi holds `this` across iswspace).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
extern "C" int __cdecl iswspace(wchar_t c);   // 0x00BFEB90 (CRT, wint_t argument)

class CWParsedItem;

class CWStringParser {
public:
    virtual ~CWStringParser();
    wchar_t*      String;                   // +0x04
    unsigned long StringPos;                // +0x08
    unsigned long Line;                     // +0x0c
    bool ReadAsStringUntilNonWhitespace(CWParsedItem* item);   // 0x00A0CC50
    bool ReadAsStringUntilWhitespace(CWParsedItem* item);      // 0x00A0CD40
    bool ReadNextChunkAsString(CWParsedItem* item);
};

bool CWStringParser::ReadNextChunkAsString(CWParsedItem* item)
{
    if (iswspace(String[StringPos]))
        return ReadAsStringUntilNonWhitespace(item);
    return ReadAsStringUntilWhitespace(item);
}