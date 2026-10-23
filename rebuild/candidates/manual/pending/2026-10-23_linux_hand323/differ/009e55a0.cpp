// CSymbolMap::ParseEnum(CStringParser&) -- parse `[name] { A, B = <expr>, C } ;` from the parser: read the optional enum type name (else "NULL"), expect '{', then add each symbol under that name with a running value (an '=' reads the value through a CPersistContext), until '}' ';'; parse errors are reported with the parser's line number
// Grade: DIFFER(1332v1317) -- the inline CCharString::operator==(const char*) materialises its result (sete al) and VC keeps the constant 1 (item type) in esi, restoring it after every repe cmpsb, where retail branches straight off the cmpsb / callee result and compares [item],1 from memory; the 'Unexpected EOF while reading enum' error is a full inline copy where retail cross-jumps into the brace-error tail. Tried: nested-if outer structure (fixed the first two error tails), ternary operator== (adds movzx), __forceinline operator== (worse)
// Leaf: ParseEnum
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E55A0, 1315 bytes, `ret 4`.
// Literals: "Cannot read Definition Type name" 0x0129C15C, "Unexpected EOF while reading enum" 0x0129C214, "NULL" 0x012393E4, "{" 0x0129B6AC,
//   "Expecting a brace in enum" 0x0129C180, "}" 0x0129B0E0, ";" 0x0123542C, "Unexpected EOF in enum def line" 0x0129C1B4,
//   "Expected string in enum" 0x0129B090, "," 0x0122E024, "=" 0x0129C1F8, "Unexpected EOF in enum" 0x0129C1FC,
//   "Expecting ',' '=' or '}' in enum" 0x0129C1D4, "Expecting ';' in enum" 0x0129C19C, "" 0x0122D70E.
// Callees: CCharString() 0x0099E4B0 / copy 0x0099EC30 / operator=(const CCharString&) 0x0099EFB0 / operator=(const char*) 0x0099EFE0 /
//   operator!=(const char*) 0x0099E960 / dtor 0x0099EAE0, string-data operator==(const char*) 0x004115A0 (the inline CCharString::operator==),
//   CStringParser::ReadNextItem 0x009BA080, CParsedItem::GetText 0x009B9870 / GetString 0x009B9880, symbol-map error report 0x009E4F80
//   (cdecl; channel object 0x013CAA34), AddSymbolDirty 0x009E54E0, CPersistContext ctor 0x009BADD0, TransferIn(long) 0x009E2590 (__fastcall).
#include <string.h>

struct CStringBuffer {
    bool operator==(const char* text) const;           // 0x004115A0
};
struct CCharString {
    CStringBuffer* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator=(const char* text);          // 0x0099EFE0
    bool operator!=(const char* text) const;           // 0x0099E960
    bool operator==(const char* text) const {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return *PStringData == text;
    }
};
struct CParsedItem {
    long Type;                                         // +0x00 (1 = symbol, 2 = string)
    long Value;                                        // +0x04
    CCharString Text;                                  // +0x08
    const CCharString& GetText() const;                // 0x009B9870
    const CCharString& GetString() const;              // 0x009B9880
};
struct CStringParser {
    unsigned char _pad00[0x24];
    long LineNumber;                                   // +0x24
    bool ReadNextItem(CParsedItem& item);              // 0x009BA080
};
struct CDefinitionManager;
struct CSymbolMap;
struct CPersistContext {
    unsigned char _data[0x2c];
    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool loading, CDefinitionManager* defs,
                    CSymbolMap* symbols, CCharString* name);   // 0x009BADD0
};
void __fastcall TransferIn(CPersistContext& context, CStringParser& parser, long* value);   // 0x009E2590
struct CWideString { unsigned short* PData; };
extern CWideString g_SymbolMapErrorChannel;            // 0x013CAA34
struct CSymbolMap {
    static void __cdecl Error(CSymbolMap* map, const CWideString* channel, long line, const char* message);   // 0x009E4F80
    void AddSymbolDirty(const CCharString& type, const CCharString& name, long value);   // 0x009E54E0
    bool ParseEnum(CStringParser& parser);
};

bool CSymbolMap::ParseEnum(CStringParser& parser)
{
    CParsedItem item;
    if (parser.ReadNextItem(item) == true) {
    CCharString typeName;
    if (item.Type == 2) {
        typeName = item.GetString();
        if (!parser.ReadNextItem(item)) {
            Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Unexpected EOF while reading enum");
            return false;
        }
    } else
        typeName = "NULL";
    if (item.Type == 1 && !(item.GetText() != "{")) {
    long value = 0;
    while (parser.ReadNextItem(item)) {
        if (item.Type == 1 && item.GetText() == "}") {
            if (!parser.ReadNextItem(item)) {
                Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Unexpected EOF in enum def line");
                return false;
            }
            if (item.Type == 1 && item.GetText() == ";")
                return true;
        }
        if (item.Type != 2)
            Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Expected string in enum");
        CCharString name(item.GetString());
        if (!parser.ReadNextItem(item)) {
            Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Unexpected EOF in enum");
            return false;
        }
        if (item.Type == 1 && item.GetText() == ",") {
            AddSymbolDirty(typeName, name, value);
        } else if (item.Type == 1 && item.GetText() == "}") {
            AddSymbolDirty(typeName, name, value);
            if (!parser.ReadNextItem(item)) {
                Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Unexpected EOF in enum");
                return false;
            }
            if (item.Type != 1 || item.GetText() != ";")
                Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Expecting ';' in enum");
            return true;
        } else if (item.Type == 1 && item.GetText() == "=") {
            CPersistContext context(&parser, 0, true, 0, this, 0);
            long assigned;
            TransferIn(context, parser, &assigned);
            value = assigned;
            AddSymbolDirty(typeName, name, value);
            if (!parser.ReadNextItem(item)) {
                Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Unexpected EOF in enum");
                return false;
            }
            if (item.Type == 1 && item.GetText() == ",") {
            } else if (item.Type == 1 && item.GetText() == "}") {
                if (!parser.ReadNextItem(item)) {
                    Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Unexpected EOF in enum");
                    return false;
                }
                if (item.Type != 1 || item.GetText() != ";")
                    Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Expecting ';' in enum");
                return true;
            } else
                Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Expecting ',' '=' or '}' in enum");
        } else
            Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Expecting ',' '=' or '}' in enum");
        ++value;
    }
    return true;
    }
    Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Expecting a brace in enum");
    return false;
    }
    Error(this, &g_SymbolMapErrorChannel, parser.LineNumber, "Cannot read Definition Type name");
    return false;
}
