// CDefinitionManager::ParseEnumForSymbols(CStringParser& parser) -- read `enum ["Prefix"] { A, B = n, C }`: an optional
// Grade: DIFFER(ours v retail) -- 1030v1017: retail caches separator.PStringData in edi across the out-of-line
//   operator== call so the "=" test needs no reload/null retest (lane gap: register caching across calls); also
//   name/separator frame words swapped (F+8/F+0xc)
// string prefix (else "NULL"), then for each entry name the separator decides its value ("," keeps the running index,
// "=" reads the value through a persist context, "}" ends the enum and must be followed by ";"); each named entry is
// added to the symbol map as prefix/name/value and the index advances past it
// Leaf: ParseEnumForSymbols
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ACEE0, length 1017 bytes, `ret 4`. Mangled (PDB):
//   ?ParseEnumForSymbols@CDefinitionManager@@IAEXAAVCStringParser@@@Z
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0 / operator=
//   0x0099EFB0 / operator=(const char*) 0x0099EFE0, operator+ 0x0099F570 / 0x0099F600 (__fastcall),
//   CBasicString<char>::operator==(const char*) 0x004115A0, CStringParser PeekNextItemType 0x009BA900 (descriptive) /
//   ReadAsString 0x009BA6F0 / ReadAsSymbol 0x009BA790 / ReadNextItem 0x009BA080 / PeekNextItem 0x009BA8C0,
//   CParsedItem::GetSymbol 0x009B9870 / GetString 0x009B9880, CPersistContext ctor 0x009BADD0, the long TransferIn
//   0x009E2590 (__fastcall), CSymbolMap::AddSymbolDirty(prefix, name, value) 0x009E54E0, Error 0x009AC9F0.
// Layout: retail CDefinitionManager PSymbolMap +0x58, ErrorCallback +0xb0 (PDB +0x60/+0xcc); CParsedItem: type +0,
//   string +8; CPersistContext 0x2c bytes (no destructor call in retail).
// Shape: CCharString::operator==(const char*) with the strcmp null-block arm (landed 0x008EDA90); the "=" test is a
//   strcmp on GetString() (the null arm folds away); each exit writes its error/cleanup in full.
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                              // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator=(const char* text);   // 0x0099EFE0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    bool EqualsChar(char c) const
    {
        if (PStringData == 0)
            return false;
        char buf[2];
        buf[0] = c;
        buf[1] = 0;
        return strcmp(PStringData->Data.PData, buf) == 0;
    }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
struct CParsedItem {
    long        Type;                           // +0x00 (1 symbol, 2 string)
    long        Value;                          // +0x04
    CCharString Text;                           // +0x08
    const CCharString& GetSymbol();             // 0x009B9870
    const CCharString& GetString();             // 0x009B9880
};
typedef void (__cdecl* TErrorCallback)(CCharString*);
struct CStringParser {
    long PeekNextItemType();                    // 0x009BA900
    CCharString ReadAsString();                 // 0x009BA6F0
    CCharString ReadAsSymbol();                 // 0x009BA790
    bool ReadNextItem(CParsedItem& item);       // 0x009BA080
    bool PeekNextItem(CParsedItem& item);       // 0x009BA8C0
};
struct CSymbolMap {
    void AddSymbolDirty(const CCharString& prefix, const CCharString& name, long value);   // 0x009E54E0
};
static inline bool EqualsText(const char* text, char c)
{
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    return strcmp(text, buf) == 0;
}
struct CDefinitionManager;
struct CPersistContext {
    unsigned char _body[0x2c];
    CPersistContext(CStringParser* parser, TErrorCallback callback, bool isNew, CDefinitionManager* manager,
                    CSymbolMap* symbols, CCharString* text);   // 0x009BADD0
};
void __fastcall TransferIn_009e2590(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
struct CDefinitionManager {
    unsigned char  _pad000[0x58];
    CSymbolMap*    PSymbolMap;                  // +0x58
    unsigned char  _pad05c[0xb0 - 0x5c];
    TErrorCallback ErrorCallback;               // +0xb0
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009AC9F0
    void ParseEnumForSymbols(CStringParser& parser);
};

void CDefinitionManager::ParseEnumForSymbols(CStringParser& parser)
{
    long index = 0;
    CCharString prefix;
    if (parser.PeekNextItemType() == 2)
        prefix = parser.ReadAsString();
    else
        prefix = "NULL";
    parser.ReadAsSymbol();
    CParsedItem item;
    while (parser.ReadNextItem(item)) {
        if (item.Type == 1 && item.GetSymbol() == "}") {
            if (!(parser.ReadAsSymbol() == ";"))
                Error(parser, "Unexpected symbol in enum.");
            return;
        }
        if (item.Type != 2) {
            Error(parser, "Expected string in enum");
            return;
        }
        CCharString name = item.GetString();
        CCharString separator = parser.ReadAsSymbol();
        if (separator == ",") {
        } else if (separator.EqualsChar('=')) {
            long value;
            CPersistContext context(&parser, ErrorCallback, true, this, PSymbolMap, 0);
            TransferIn_009e2590(&context, &parser, &value);
            index = value;
        } else if (separator == "}") {
            if (!(parser.ReadAsSymbol() == ";"))
                Error(parser, "Unexpected symbol in enum.");
            return;
        } else {
            Error(parser, CCharString("Unknown symbol after '") + name + "'.");
            return;
        }
        PSymbolMap->AddSymbolDirty(prefix, name, index);
        if (parser.PeekNextItem(item) && item.Type == 1 && item.GetSymbol() == ",")
            parser.ReadNextItem(item);
        ++index;
    }
}
