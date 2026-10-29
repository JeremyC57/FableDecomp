// CPersistContext::TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item) -- text load of a named object: on a copy of the context's parser, for every occurrence of the name (until the context is finished) let the object read itself, then require the closing ";" (cleaning the consumed text out of the source string with spaces, newlines kept) or report the error
// Leaf: ?TransferObjectLoadText@CPersistContext@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BBA40, 417 bytes + padding (`ret 8`). Callees: CStringParser(const CStringParser&) @ 0x0046381B,
// CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0, CStringParser 0x009B9C00 (descriptive SkipToItem), ReadAsSymbol
// @ 0x009BA790, CBasicString<char>::operator== @ 0x004115A0, CPersistContext::Error @ 0x009BAA90, the comment list dtor 0x004046B0,
// ~CBaseClass 0x0099A2E0; the item's TransferIn is its vtable slot 1. Literals: "" 0x0122D70E, ";" 0x0123542C, the error message
// @ 0x01235404 (text not recovered; the literal here is a placeholder, relocation-masked).
// Layout: CPersistContext per Ego_r.pdb (as the landed TransferMap 0x00757160); CStringParser retail 0x28 bytes (landed copy ctor
// 0x0046381B); CCharString as the landed CCombatAction_FallStunned ctor 0x008EDA90.
// Shape: the loop condition converts the name IMPLICITLY (CCharString(const char*, long = -1)): an explicit CCharString(name, -1)
// pushes the constructor's returned eax and swaps the temporary's frame slot with the error message's; the message is a named
// local of the else block; the cleaning loop's end is a local read before the PStringToClean test.
#include <string.h>
#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long             NoRefs;
};
#pragma pack(pop)
extern char g_emptyLiteral_0129aaf4[];          // 0x0129AAF4: ""
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->Data.PData; }
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CBaseClass {
public:  virtual ~CBaseClass(); };             // dtor 0x0099A2E0
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); };   // 0x004046B0
struct CCursorPos {
    bool InQuotes;                              // +0x00
    long StringPos;                             // +0x04
    long Line;                                  // +0x08
};
class CStringParser : public CBaseClass { public:                  // retail 0x28 bytes
    char*                    String;            // +0x04
    unsigned long            BaseLineOffset;    // +0x08
    CCharString              Filename;          // +0x0c
    CCharString              TempCharString;    // +0x10
    CStringParserCommentList Comments;          // +0x14
    void (*ErrorCallback)(CCharString*);        // +0x18
    CCursorPos               CursorPos;         // +0x1c
    CStringParser(const CStringParser& other);  // 0x0046381B
    bool SkipToItem(const CCharString& name);   // 0x009B9C00 (descriptive)
    CCharString ReadAsSymbol();                 // 0x009BA790
};
class CPersistContext;
struct CPersistContext_TransferableObjectBase {
    virtual void v00();
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);   // slot 1 (+4)
};
class CDefinitionManager;
class CSymbolMap;
class CMemoryDataInputStream;
class CDataOutputStream;
class CPersistContext { public:
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__cdecl*          ErrorCallback)(CCharString*); // +0x0c
    CCharString*            PStringToClean;               // +0x10
    bool                    UseDefaults;                  // +0x14
    bool                    UseCRCCheck;                  // +0x15
    bool                    SingleTransfer;               // +0x16
    bool                    Finished;                     // +0x17
    long                    Mode;                         // +0x18
    bool                    Binary;                       // +0x1c
    CCharString*            PStringToSaveTo;              // +0x20
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item);
};

void CPersistContext::TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item)
{
    CStringParser parser(*PParser);
    long nameLength = strlen(name);
    while (!Finished && parser.SkipToItem(name)) {
        long start = parser.CursorPos.StringPos - nameLength;
        item.TransferIn(this, &parser);
        if (SingleTransfer)
            Finished = true;
        if (parser.ReadAsSymbol() == ";") {
            long end = parser.CursorPos.StringPos;
            if (PStringToClean) {
                char* text = PStringToClean->GetString();
                for (long i = start; i < end; ++i)
                    if (text[i] != '\n')
                        text[i] = ' ';
            }
        } else {
            CCharString message("Expected ';'", -1);
            Error(parser, message);
        }
    }
}