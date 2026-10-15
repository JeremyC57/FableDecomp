// CStringParser::CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename, long baseLineOffset) -- build a parser over a CCharString
// Leaf: ??0CStringParser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404720, 117 bytes (`ret 10h`). Callees: CBase::CBase 0x0099A2D0, CCharString::CCharString
// 0x0099E4B0 (Filename, TempCharString), malloc 0x00BFEA0E (the comment list's 16-byte head node), CCharString::operator=
// 0x0099EFB0, CStringParser::Init(char*, callback, const CCharString&, long) 0x009B9530.
//
// The string parser reads our text data files (defs, scripts, config) and tracks the cursor and line number for
// error messages. The char* Init does the real work but does not own the characters it walks, so when a caller
// hands us a CCharString we keep a reference-counted copy in TempCharString for the parser's lifetime and then
// initialise over that string's characters (an empty string parses the shared "" literal). The error callback,
// the filename and the base line offset go straight through to Init.
//
// Matching: retail CStringParser is 0x28 bytes (the PDB's 0x30 has a 12-byte list): the comment list is one word,
// an STLport list whose head node is allocated self-linked in the member constructor (zeroed first, then
// malloc(16) into a LOCAL that is linked to itself and stored once); the vtable (0x0122D874) is stored after the polymorphic CBase constructor;
// the characters come from the ARGUMENT (kept in ebx) through the null-case-first inline accessor, so the ""
// literal is the fall-through arm; TempCharString's address lives in edi across the two calls on it.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CBase {
public:
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
};

struct CCharStringData {                           // shared representation block of a CCharString
    char*         PData;                           // characters
    unsigned long Length;                          // character count
};
extern char g_emptyLiteral_0129aaf4[];             // "" sentinel returned for an empty string

class CCharString {
public:
    CCharStringData* PStringData;                  // null means empty
    CCharString();                                 // 0x0099E4B0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0: share the other string's block
    char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->PData; }
};

struct SCommentNode {                              // list node: links first, the CStringParserCommentBoundaries payload after
    SCommentNode* Next;
    SCommentNode* Prev;
    long          Start;
    long          End;
};

struct CCommentList {                              // STLport list<CStringParserCommentBoundaries>: one word, the head node
    SCommentNode* Node;
    CCommentList() : Node(0)
    {
        SCommentNode* node = (SCommentNode*)malloc(sizeof(SCommentNode));
        node->Next = node;
        node->Prev = node;
        Node = node;
    }
};

typedef void (*StringParserErrorCallback)(CCharString*);

class CStringParser : public CBase {               // vtable 0x0122D874
public:
    char*                     String;              // +0x04 characters being parsed (set by Init)
    unsigned long             BaseLineOffset;      // +0x08 line number of the first character (set by Init)
    CCharString               Filename;            // +0x0c for error messages
    CCharString               TempCharString;      // +0x10 our reference to the text when given a CCharString
    CCommentList              Comments;            // +0x14 comment boundaries found so far
    StringParserErrorCallback ErrorCallback;       // +0x18
    long                      CursorPos[3];        // +0x1c InQuotes, StringPos, Line

    CStringParser(const CCharString& text, StringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);
    void Init(char* text, StringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530
};

CStringParser::CStringParser(const CCharString& text, StringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset)
    : CBase(), Filename(), TempCharString(), Comments()
{
    TempCharString = text;
    Init(text.GetString(), errorCallback, filename, baseLineOffset);
}
