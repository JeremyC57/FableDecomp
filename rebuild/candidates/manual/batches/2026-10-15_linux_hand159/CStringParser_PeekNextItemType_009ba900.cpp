// CStringParser::PeekNextItemType() -- parse the next item without consuming it and return its type (PIT_NONE if nothing parses)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BA900, 101 bytes. Callees: CCharString() @ 0x0099E4B0, ReadNextItem @ 0x009BA080, ~CCharString @ 0x0099EAE0.
// Shape: the 12-byte cursor is saved in registers/frame before the read and copied back on success.
extern "C" int __cdecl isspace(int c);   // 0x00BFEDBE (CRT)
extern "C" int __cdecl isdigit(int c);   // 0x00BFEDA6 (CRT)
extern "C" int __cdecl isalpha(int c);   // 0x00BFEDB8 (CRT)
extern "C" char* __cdecl strstr(const char* s, const char* find);   // 0x00BFEB3A (CRT)
struct CCharString {                     // one CCharStringData pointer
    void* PStringData;
    CCharString();                       // 0x0099E4B0
    CCharString(const char* text, long length);   // 0x0099EBF0
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                      // 0x0099EAE0
    operator const char*() const;        // 0x0099E4C0
};
struct CCursorPos {                      // PDB: InQuotes / StringPos / Line (12 bytes)
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
enum EParsedItemType { PIT_NONE = 0, PIT_SYMBOL = 1, PIT_STRING = 2, PIT_INTEGER = 3, PIT_FLOAT = 4 };
struct CParsedItem {                     // PDB: Type, a union of the numeric value, String
    EParsedItemType Type;
    union { long Integer; float Float; };
    CCharString     String;
};
// Retail layout (0x28 bytes; the Ego_r PDB's debug-build layout is 0x30, its list is 12 bytes there):
struct CStringParser {
    virtual ~CStringParser();
    char*          String;               // +0x04
    unsigned long  BaseLineOffset;       // +0x08
    CCharString    Filename;             // +0x0c
    CCharString    TempCharString;       // +0x10
    void*          Comments;             // +0x14 STLport list node pointer
    void (__fastcall* ErrorCallback)(const CCharString&);   // +0x18
    CCursorPos     CursorPos;            // +0x1c
    long SkipWhitespace();                                  // 0x009B9C60
    bool ReadNextItem(CParsedItem& item);                   // 0x009BA080
    long ReadAsInteger();
    float ReadAsFloat();
    CCharString ReadAsString();
    CCharString ReadAsSymbol();
    EParsedItemType PeekNextItemType();
protected:
    void Error(const CCharString& message);                 // 0x009B95F0
    bool ReadNextNumber(CParsedItem& item);                 // 0x009B9EE0
    bool ReadNextString(CParsedItem& item);                 // 0x009B9FC0
    bool ReadNextSymbol(CParsedItem& item);                 // 0x009B99C0
};

EParsedItemType CStringParser::PeekNextItemType()
{
    CParsedItem item;
    CCursorPos saved = CursorPos;
    if (ReadNextItem(item)) {
        CursorPos = saved;
        return item.Type;
    }
    return PIT_NONE;
}
