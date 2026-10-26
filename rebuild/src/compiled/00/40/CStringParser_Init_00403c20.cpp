// CStringParser::Init(const CCharString& text, callback, const CCharString& filename, long baseLineOffset) -- keep our own copy of the text, then parse from its characters
// Leaf: ?Init@CStringParser@@QAEXABVCCharString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403C20, 60 bytes (+4 int3 padding). Callees: CCharString::operator=
// (const CCharString&) @ 0x0099EFB0, CStringParser::Init(char*, callback, const CCharString&, long) @ 0x009B9530.
//
// The string parser is what reads our text-format data files (defs, scripts, config): it walks a raw
// character buffer, tracks the cursor position and line number for error messages, and skips comments.
// The char* overload does all of that but does not own the buffer, so when a caller hands us a
// CCharString we first take a reference-counted copy into TempCharString, which keeps the characters
// alive for as long as the parser does, and then initialise over that string's characters. The error
// callback, the filename (for messages) and the base line offset (for text embedded in a larger file)
// are passed straight through.
//
// Matching: retail CStringParser is 0x28 bytes, not the PDB's 0x30 (engine/CStringParser.h): the comment
// list is ONE word here (head pointer, 16-byte self-linked head node) where the PDB has 12 bytes, so
// ErrorCallback sits at +0x18 and CursorPos (InQuotes, StringPos, Line) at +0x1C; the class is therefore
// modelled locally with the PDB member names (CBase root with vtable 0x0122D874). The characters come
// from the ARGUMENT (re-read through edi after the assignment) via the inline null-case-first accessor
// (empty literal 0x0129AAF4 for a null string); the remaining arguments are forwarded.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CBase {
public:
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
};

// Shared representation block of a CCharString.
class CCharStringData { public:
    char*         PData;                           // characters
    unsigned long Length;                          // character count
};
extern char g_emptyLiteral_0129aaf4[];             // "" sentinel returned for an empty string

class CCharString {
public:
    CCharStringData* PStringData;                  // null means empty
    CCharString();                                 // 0x0099E4B0
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString(const char* text, long length);    // 0x0099EBF0 (-1 = strlen)
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0: share the other string's block
    char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->PData; }
};

// The comment ranges found while parsing (list<CStringParserCommentBoundaries>): head pointer only.
struct CStringParserCommentList {
    struct SNode { SNode* Next; SNode* Prev; char* PStart; char* PEnd; };
    SNode* m_pHead;                                // self-linked sentinel node
    CStringParserCommentList() : m_pHead(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->Next = node;
        node->Prev = node;
        m_pHead = node;
    }
    CStringParserCommentList(const CStringParserCommentList& other);   // 0x00463871
};

// Where the parser currently is in the text.
struct CCursorPos {
    bool          InQuotes;                        // inside a quoted string
    unsigned long StringPos;                       // character index
    unsigned long Line;                            // line number for error messages
};

class CParsedItem;
typedef void (*TStringParserErrorCallback)(CCharString* message);

class CStringParser : public CBase {               // vtable 0x0122D874
public:
    char*                      String;             // the characters being parsed (not owned)
    unsigned long              BaseLineOffset;     // added to Line when the text sits inside a larger file
    CCharString                Filename;           // named in error messages
    CCharString                TempCharString;     // our copy of the text, so String stays valid
    CStringParserCommentList   Comments;           // comment ranges to skip
    TStringParserErrorCallback ErrorCallback;      // where parse errors go
    CCursorPos                 CursorPos;          // current position
    // The real initialiser: parses from a raw buffer the caller keeps alive.
    void Init(char* text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530
    void Init(const CCharString& text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);
};

void CStringParser::Init(const CCharString& text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset)
{
    // Hold a reference to the text so the raw pointer we parse from cannot go away under us.
    TempCharString = text;
    Init(text.GetString(), errorCallback, filename, baseLineOffset);
}
