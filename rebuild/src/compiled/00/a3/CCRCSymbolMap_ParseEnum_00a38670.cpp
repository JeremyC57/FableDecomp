// CCRCSymbolMap::ParseEnum(CStringParser* parser, const CCharString& wantedName) -- parse one "enum [Name] { A, B = n, ... };" block into the CRC symbol map (skip to the next "enum" when the name is not the wanted one)
// Leaf: ParseEnum
// Retail 0x00A38670, 1456 bytes, `ret 8`.
// Callees: CCharString() 0x0099E4B0 / copy 0x0099EC30 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 /
//   operator=(const CCharString&) 0x0099EFB0 / operator=(const char*) 0x0099EFE0 / operator!=(const char*) 0x0099E960 /
//   operator!=(const CCharString&) 0x0099E900, CBasicString<char>::operator==(const char*) 0x004115A0 (the inline
//   CCharString == const char*), CStringParser::ReadNextItem 0x009BA080 / SkipUntilString 0x009B9A20,
//   CParsedItem::GetString 0x009B9880 / GetSymbol 0x009B9870, CPersistContext(parser, callback, bool, defs, symbols,
//   text) 0x009BADD0 (no destructor), the long TransferIn 0x009E2590 (`__fastcall`), CCRCSymbolMap::AddSymbolDirty
//   0x00A385A0 (FableWin.pdb / Ego_r.pdb ?AddSymbolDirty@CCRCSymbolMap@@QAEXABVCCharString@@J@Z), CCRCSymbolMap::Error 0x00A381D0 (member varargs, `this` on the stack) with the global file name 0x013D27E8.
// Layout: CStringParser retail 0x28 bytes (the landed ParseBuffer 0x00A38E50 model; the engine header carries the
//   12-byte PDB list): the error line is the dword at +0x24. CPersistContext per Ego_r.pdb (0x2c bytes).
#include <string.h>
#pragma intrinsic(strcmp)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator=(const char* text);          // 0x0099EFE0
    bool operator!=(const char* text) const;           // 0x0099E960
    bool operator!=(const CCharString& other) const;   // 0x0099E900
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CRetailWideString { void* PStringData; };
extern CRetailWideString g_CRCSymbolFileName;          // 0x013D27E8
struct CParsedItem {                                   // 12 bytes
    long        Type;                                  // 1 = symbol, 2 = identifier
    long        Value;
    CCharString Text;
    const CCharString& GetString();                    // 0x009B9880
    const CCharString& GetSymbol();                    // 0x009B9870
};
struct CStringParser {                                 // retail 0x28 bytes
    void*         vfptr;                               // +0x00
    char*         String;                              // +0x04
    unsigned long BaseLineOffset;                      // +0x08
    CCharString   Filename;                            // +0x0c
    CCharString   TempCharString;                      // +0x10
    void*         Comments;                            // +0x14
    void*         ErrorCallback;                       // +0x18
    long          CursorOffset;                        // +0x1c
    long          CursorColumn;                        // +0x20
    long          CursorLine;                          // +0x24
    bool ReadNextItem(CParsedItem& item);              // 0x009BA080
    bool SkipUntilString(const CCharString& text);     // 0x009B9A20
};
struct CDefinitionManager;
struct CSymbolMap;
struct CPersistContext {                               // 0x2c bytes
    CDefinitionManager* PDefinitionManager;
    CSymbolMap*         PSymbolMap;
    CStringParser*      PParser;
    void*               ErrorCallback;
    CCharString*        PStringToClean;
    bool                UseDefaults, UseCRCCheck, SingleTransfer, Finished;
    long                Mode;
    bool                Binary;
    CCharString*        PStringToSaveTo;
    void*               PLoadStream;
    void*               PSaveStream;
    CPersistContext(CStringParser* parser, void (*callback)(CCharString*), bool reportErrors,
                    CDefinitionManager* definitions, CSymbolMap* symbols, CCharString* text);   // 0x009BADD0
};
void __fastcall TransferInLong_009e2590(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
struct CCRCSymbolMap {
    void Error(const CRetailWideString& file, long line, const char* format, ...);   // 0x00A381D0
    void AddSymbolDirty(const CCharString& name, long value);                       // 0x00A385A0
    bool ParseEnum(CStringParser* parser, const CCharString& wantedName);
};

bool CCRCSymbolMap::ParseEnum(CStringParser* parser, const CCharString& wantedName)
{
    CParsedItem item;
    if (parser->ReadNextItem(item) == true) {
    CCharString enumName;
    if (item.Type == 2) {
        enumName = item.GetString();
        if (!parser->ReadNextItem(item)) {
            Error(g_CRCSymbolFileName, parser->CursorLine, "Unexpected EOF while reading enum");
            return false;
        }
    } else {
        enumName = "NULL";
    }
    if (wantedName != "" && wantedName != enumName) {
        CCharString keyword("enum");
        parser->SkipUntilString(keyword);
        return true;
    }
    if (item.Type != 1 || item.GetSymbol() != "{") {
        Error(g_CRCSymbolFileName, parser->CursorLine, "Expecting a brace in enum");
        return false;
    }
    for (long value = 0; parser->ReadNextItem(item); ++value) {
        if (item.Type == 1 && item.GetSymbol() == "}") {
            if (!parser->ReadNextItem(item)) {
                Error(g_CRCSymbolFileName, parser->CursorLine, "Unexpected EOF in enum def line");
                return false;
            }
            if (item.Type == 1 && item.GetSymbol() == ";")
                return true;
        }
        if (item.Type != 2)
            Error(g_CRCSymbolFileName, parser->CursorLine, "Expected string in enum");
        CCharString valueName(item.GetString());
        if (!parser->ReadNextItem(item)) {
            Error(g_CRCSymbolFileName, parser->CursorLine, "Unexpected EOF in enum");
            return false;
        }
        if (item.Type == 1 && item.GetSymbol() == ",") {
            AddSymbolDirty(valueName, value);
        } else if (item.Type == 1 && item.GetSymbol() == "}") {
            AddSymbolDirty(valueName, value);
            if (!parser->ReadNextItem(item)) {
                Error(g_CRCSymbolFileName, parser->CursorLine, "Unexpected EOF in enum");
                return false;
            }
            if (item.Type != 1 || item.GetSymbol() != ";")
                Error(g_CRCSymbolFileName, parser->CursorLine, "Expecting ';' in enum");
            break;
        } else if (item.Type == 1 && item.GetSymbol() == "=") {
            CPersistContext context(parser, 0, true, 0, 0, 0);
            long read;
            TransferInLong_009e2590(&context, parser, &read);
            value = read;
            AddSymbolDirty(valueName, read);
            if (!parser->ReadNextItem(item)) {
                Error(g_CRCSymbolFileName, parser->CursorLine, "Unexpected EOF in enum");
                return false;
            }
            if (item.Type == 1) {
                if (item.GetSymbol() == ",") {
                } else if (item.GetSymbol() == "}") {
                    if (!parser->ReadNextItem(item)) {
                        Error(g_CRCSymbolFileName, parser->CursorLine, "Unexpected EOF in enum");
                        return false;
                    }
                    if (item.Type != 1 || item.GetSymbol() != ";")
                        Error(g_CRCSymbolFileName, parser->CursorLine, "Expecting ';' in enum");
                    return true;
                } else {
                    Error(g_CRCSymbolFileName, parser->CursorLine, "Expecting ',' '=' or '}' in enum");
                }
            } else {
                Error(g_CRCSymbolFileName, parser->CursorLine, "Expecting ',' '=' or '}' in enum");
            }
        } else {
            Error(g_CRCSymbolFileName, parser->CursorLine, "Expecting ',' '=' or '}' in enum");
        }
    }
    return true;
    }
    Error(g_CRCSymbolFileName, parser->CursorLine, "Cannot read Definition Type name");
    return false;
}