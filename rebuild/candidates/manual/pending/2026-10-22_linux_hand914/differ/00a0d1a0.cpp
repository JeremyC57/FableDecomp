// CWStringParser::ReadAsString() -- reads the next item and returns its text when it is a string item (type 2); otherwise throws CGenericException
// Grade: DIFFER(96v95) -- INT3-ONLY: byte-identical (RELOCATION_MATCH) once the oracle keeps the compiler int3 after the noreturn _CxxThrowException call (retail 96 bytes: the next function starts right after it); the chunk oracle trimmed it as padding
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D1A0, 95 bytes, `ret 4` (hidden CWideString result). Callees:
// CWideString::CWideString() 0x0099AED0, CWStringParser::ReadNextItem(CWParsedItem*) 0x00A0CFC0,
// CWideString::CWideString(const CWideString&) 0x0099B720, ~CWideString 0x0099B510,
// _CxxThrowException 0x00BFEB84 with ThrowInfo 0x013692F8 (.?AVCGenericException@@ : CExceptionBase).
// The wide twin of the landed CStringParser::ReadAsString 0x009BA6F0, which reports an error instead of throwing.
// Shape: a CWParsedItem local (Type +0, value +4, String +8 -- only the string has a constructor); `return
// item.String;` copies into the hidden result before the item's destructor; the throw path is placed last,
// the empty exception object is copied byte-wise from its dead slot (the hidden-result argument slot),
// and there is no EH frame (the TU is built without /EHsc, so the item is not unwound).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                                   // 0x0099AED0
    CWideString(const CWideString& other);           // 0x0099B720
    ~CWideString();                                  // 0x0099B510
};

struct CWParsedItem {
    long        Type;      // +0x00 EParsedItemType (1 = symbol, 2 = string)
    long        Integer;   // +0x04
    CWideString String;    // +0x08
};

class CExceptionBase { };
class CGenericException : public CExceptionBase {
public:
    CGenericException() { }
};

struct CWStringParser {
    void*          __vftable;  // +0x00
    wchar_t*       String;     // +0x04
    unsigned long  StringPos;  // +0x08
    unsigned long  Line;       // +0x0C
    bool ReadNextItem(CWParsedItem* item);           // 0x00A0CFC0
    CWideString ReadAsString();
};

CWideString CWStringParser::ReadAsString()
{
    CWParsedItem item;
    if (ReadNextItem(&item) && item.Type == 2)
        return item.String;
    throw CGenericException();
}
