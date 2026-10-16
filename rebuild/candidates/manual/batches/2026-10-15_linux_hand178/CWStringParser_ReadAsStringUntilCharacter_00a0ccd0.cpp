// CWStringParser::ReadAsStringUntilCharacter(wchar_t) -- by-value convenience overload returning the read text
// Leaf: ?ReadAsStringUntilCharacter@CWStringParser@@QAE?AVCWideString@@_W@Z
// Retail: Fable.exe @ 0x00A0CCD0, 98 bytes (+int3). PDB ?ReadAsStringUntilCharacter@CWStringParser@@QAE?AVCWideString@@_W@Z
// Callees: CWideString::CWideString() 0x0099AED0 (manifest label "CCharString::CCharString" is wrong: it builds the
// item's +8 CWideString), ReadAsStringUntilCharacter(wchar_t, CWParsedItem*) 0x00A0CBB0, CWideString copy ctor
// 0x0099B720, CWideString(const wchar_t*) 0x0099B6B0, ~CWideString 0x0099B510.
// A 12-byte CWParsedItem local (only its CWideString member has a constructor) is filled by the item overload;
// on success its string is copied into the hidden result, otherwise the result is L"". No EH frame in retail.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                          // 0x0099AED0
    CWideString(const CWideString& other);  // 0x0099B720
    CWideString(const wchar_t* text);       // 0x0099B6B0
    ~CWideString();                         // 0x0099B510
};

struct CWParsedItem {
    long        Type;      // +0x00 EParsedItemType
    long        Integer;   // +0x04
    CWideString String;    // +0x08
    const CWideString& GetString() const { return String; }
};

struct CWStringParser {
    void*          __vftable;  // +0x00
    wchar_t*       String;     // +0x04
    unsigned long  StringPos;  // +0x08
    unsigned long  Line;       // +0x0C
    bool ReadAsStringUntilCharacter(wchar_t stop, CWParsedItem* item);   // 0x00A0CBB0
    CWideString ReadAsStringUntilCharacter(wchar_t stop);
};

CWideString CWStringParser::ReadAsStringUntilCharacter(wchar_t stop)
{
    CWParsedItem item;
    if (ReadAsStringUntilCharacter(stop, &item))
        return item.GetString();
    return CWideString(L"");
}
