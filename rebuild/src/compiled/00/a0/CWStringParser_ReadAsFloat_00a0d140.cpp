// CWStringParser::ReadAsFloat() -- read the next item and return its value when it is a float item (type 4); otherwise throw CGenericException
// Bounds: int3-after-noreturn row, landed against the N+1 span: int3-after-noreturn: byte-identical to retail up to the final _CxxThrowException call; VC7.1 emits one 0xCC after the noreturn call, which the cutter stripped from the row (land against the 88-byte span, as the landed ReadAsString 0x00A0D1A0 / ReadAsSymbol 0x00A0D200 twins).
// Leaf: ?ReadAsFloat@CWStringParser@@QAEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D140, 87 bytes to the final noreturn call (+9 int3 padding), thiscall, `ret`.
//   PDB name ?ReadAsFloat@CWStringParser@@QAEMXZ; the catalog label "ReadAsInteger" is propagated from the 0x00A0D0E0 twin
//   (this body returns through `fld`, tests item type 4 = float).
// Callees: CWideString::CWideString() 0x0099AED0, CWStringParser::ReadNextItem(CWParsedItem*) 0x00A0CFC0 (landed),
//   ~CWideString 0x0099B510, _CxxThrowException 0x00BFEB84 with ThrowInfo 0x013692F8 (.?AVCGenericException@@).
// Layout: Ego_r.pdb CWStringParser (vptr, String +0x04, StringPos +0x08, Line +0x0c); CWParsedItem (12 bytes):
//   Type +0, value +4, String +8.
// Shape: the float twin of the landed ReadAsString 0x00A0D1A0 / ReadAsSymbol 0x00A0D200: a CWParsedItem local (only the
//   string has a constructor); `this` is dead after ReadNextItem so esi is popped before the tests; the float value is
//   parked in its own frame word with an integer move across the item's destructor and loaded with `fld` at the end;
//   the throw path is placed last with the empty exception object copied byte-wise onto its own frame byte; no EH frame.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();                                   // 0x0099AED0
    ~CWideString();                                  // 0x0099B510
};

class CWParsedItem {
public:
    long        Type;                       // +0x00 EParsedItemType (3 = integer, 4 = float)
    union { long Integer; float Float; } Value;      // +0x04
    CWideString String;                     // +0x08
};

class CExceptionBase { };
class CGenericException : public CExceptionBase {
public:
    CGenericException() { }
};

class CWStringParser {
public:
    virtual ~CWStringParser();
    wchar_t*      String;                   // +0x04
    unsigned long StringPos;                // +0x08
    unsigned long Line;                     // +0x0c
    bool ReadNextItem(CWParsedItem* item);  // 0x00A0CFC0
    float ReadAsFloat();
};

float CWStringParser::ReadAsFloat()
{
    CWParsedItem item;
    if (ReadNextItem(&item) && item.Type == 4)
        return item.Value.Float;
    throw CGenericException();
}