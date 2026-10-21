// Behaviour test for 009bba40 (CPersistContext::TransferObjectLoadText): every "name ... ;" occurrence is transferred and blanked
// in the clean string (newlines kept); a missing ';' reports an error; SingleTransfer stops after the first.
#include <cstdio>
#include <cstring>
#include <string>
#ifndef _MSC_VER
#define __cdecl
#endif
// CPersistContext::TransferObjectLoadText(const char* name, CPersistContext_TransferableObjectBase& item) -- text load of a named object: on a copy of the context's parser, for every occurrence of the name (until the context is finished) let the object read itself, then require the closing ";" (cleaning the consumed text out of the source string with spaces, newlines kept) or report the error
// Leaf: ?TransferObjectLoadText@CPersistContext@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BBA40, 417 bytes + padding (`ret 8`). Callees: CStringParser(const CStringParser&) @ 0x0046381B,
// CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0, CStringParser 0x009B9C00 (descriptive SkipToItem), ReadAsSymbol
// @ 0x009BA790, CBasicString<char>::operator== @ 0x004115A0, CPersistContext::Error @ 0x009BAA90, the comment list dtor 0x004046B0,
// ~CBase 0x0099A2E0; the item's TransferIn is its vtable slot 1. Literals: "" 0x0122D70E, ";" 0x0123542C, the error message
// @ 0x01235404 (text not recovered; the literal here is a placeholder, relocation-masked).
// Layout: CPersistContext per Ego_r.pdb (as the landed TransferMap 0x00757160); CStringParser retail 0x28 bytes (landed copy ctor
// 0x0046381B); CCharString as the landed CCombatAction_FallStunned ctor 0x008EDA90.
// Shape: the loop condition converts the name IMPLICITLY (CCharString(const char*, long = -1)): an explicit CCharString(name, -1)
// pushes the constructor's returned eax and swaps the temporary's frame slot with the error message's; the message is a named
// local of the else block; the cleaning loop's end is a local read before the PStringToClean test.
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
struct CBase { virtual ~CBase(); };             // dtor 0x0099A2E0
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); };   // 0x004046B0
struct CCursorPos {
    bool InQuotes;                              // +0x00
    long StringPos;                             // +0x04
    long Line;                                  // +0x08
};
struct CStringParser : CBase {                  // retail 0x28 bytes
    char*                    String;            // +0x04
    unsigned long            BaseLineOffset;    // +0x08
    CCharString              Filename;          // +0x0c
    CCharString              TempCharString;    // +0x10
    CStringParserCommentList Comments;          // +0x14
    void (*ErrorCallback)(CCharString*);        // +0x18
    CCursorPos               CursorPos;         // +0x1c
    CStringParser(const CStringParser& other);  // 0x0046381B
    CStringParser(char* s) : String(s), BaseLineOffset(0), Filename("", -1), TempCharString("", -1), ErrorCallback(0)   // test only
    { Comments.head = 0; CursorPos.InQuotes = false; CursorPos.StringPos = 0; CursorPos.Line = 0; }
    bool SkipToItem(const CCharString& name);   // 0x009B9C00 (descriptive)
    CCharString ReadAsSymbol();                 // 0x009BA790
};
struct CPersistContext;
struct CPersistContext_TransferableObjectBase {
    virtual void v00();
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);   // slot 1 (+4)
};
struct CDefinitionManager;
struct CSymbolMap;
struct CMemoryDataInputStream;
struct CDataOutputStream;
struct CPersistContext {
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

char g_emptyLiteral_0129aaf4[] = "";
bool CBasicStringChar::operator==(const char* text) const { return std::strcmp(PData, text) == 0; }
CCharString::CCharString(const char* text, long)
{
    PStringData = new CCharStringData;
    size_t n = std::strlen(text);
    PStringData->Data.PData = new char[n + 1];
    std::memcpy(PStringData->Data.PData, text, n + 1);
    PStringData->Data.StringLength = (unsigned long)n;
}
CCharString::~CCharString() { if (PStringData) { delete[] PStringData->Data.PData; delete PStringData; } }
CBase::~CBase() {}
CStringParserCommentList::~CStringParserCommentList() {}
CStringParser::CStringParser(const CStringParser& o)
    : CBase(), String(o.String), BaseLineOffset(o.BaseLineOffset), Filename("", -1), TempCharString("", -1), ErrorCallback(o.ErrorCallback)
{
    Comments.head = 0; CursorPos = o.CursorPos;
}
static void SkipSpaces(const char* s, long& p) { while (s[p] == ' ' || s[p] == '\n') ++p; }
bool CStringParser::SkipToItem(const CCharString& name)
{
    const char* found = std::strstr(String + CursorPos.StringPos, name.GetString());
    if (!found) return false;
    CursorPos.StringPos = (long)(found - String) + (long)std::strlen(name.GetString());
    return true;
}
CCharString CStringParser::ReadAsSymbol()
{
    SkipSpaces(String, CursorPos.StringPos);
    char buf[2] = { String[CursorPos.StringPos], 0 };
    if (buf[0]) ++CursorPos.StringPos;
    return CCharString(buf, -1);
}
void CPersistContext_TransferableObjectBase::v00() {}
void CPersistContext_TransferableObjectBase::TransferIn(CPersistContext*, CStringParser*) {}
static int s_items = 0, s_errors = 0;
struct Item : CPersistContext_TransferableObjectBase {
    void v00() {}
    void TransferIn(CPersistContext*, CStringParser* parser)
    {   // consume " <digits>"
        ++s_items;
        SkipSpaces(parser->String, parser->CursorPos.StringPos);
        while (parser->String[parser->CursorPos.StringPos] >= '0' && parser->String[parser->CursorPos.StringPos] <= '9') ++parser->CursorPos.StringPos;
    }
};
void CPersistContext::Error(const CStringParser&, const CCharString&) { ++s_errors; }
static int fails = 0;
static void check(bool c, int n) { if (!c) { std::printf("fail %d\n", n); ++fails; } }
static void Reset(CPersistContext& c, CStringParser* p, CCharString* clean, bool single)
{
    std::memset(&c, 0, sizeof(c));
    c.PParser = p; c.PStringToClean = clean; c.SingleTransfer = single;
    s_items = 0; s_errors = 0;
}
int main()
{
    char text[] = "Foo 12;\nBar 3;\nFoo 45\n;Foo 7 x";
    CStringParser source(text);
    CCharString clean(text, -1);
    CPersistContext ctx;
    Item item;
    Reset(ctx, &source, &clean, false);
    ctx.TransferObjectLoadText("Foo", item);
    check(s_items == 3 && s_errors == 1, 1);                 // third Foo has no ';' (reads 'x')
    check(std::strcmp(clean.GetString(), "       \nBar 3;\n      \n Foo 7 x") == 0, 2);
    check(source.CursorPos.StringPos == 0, 3);              // the context's parser is untouched (copy)
    check(!ctx.Finished, 4);
    CCharString clean2(text, -1);
    Reset(ctx, &source, &clean2, true);
    ctx.TransferObjectLoadText("Foo", item);
    check(s_items == 1 && s_errors == 0 && ctx.Finished, 5);
    check(std::strncmp(clean2.GetString(), "       \nBar", 11) == 0, 6);
    Reset(ctx, &source, 0, false);
    ctx.TransferObjectLoadText("Bar", item);
    check(s_items == 1 && s_errors == 0, 7);
    Reset(ctx, &source, 0, false);
    ctx.Finished = true;
    ctx.TransferObjectLoadText("Foo", item);
    check(s_items == 0, 8);
    if (fails == 0) std::printf("009bba40_TEST PASS\n");
    return fails;
}