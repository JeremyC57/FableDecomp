// CStringParser::CStringParser(char*, void (*)(CCharString*), const CCharString&, long) -- constructs the parser members and initialises it over the given text
// Leaf: ??0CStringParser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00414E30, 88 bytes. Callees: CBase::CBase @ 0x0099A2D0,
// CCharString::CCharString() @ 0x0099E4B0 (Filename, TempCharString), malloc @ 0x00BFEA0E (the comment list's head
// node), CStringParser::Init(char*, callback, const CCharString&, long) @ 0x009B9530. Manifest label
// "CStringParser::ConstructorInitialize".
// Layout: retail CStringParser is 0x28 bytes, not the PDB's 0x30 (engine/CStringParser.h): the comment list is ONE word
// here (head pointer, 16-byte self-linked head node) where the PDB has 12 bytes, so ErrorCallback sits at +0x18 and
// CursorPos (InQuotes, StringPos, Line) at +0x1C. The header disagrees with the bytes, so the class is modelled locally
// with the PDB member names (CBase root with the vtable 0x0122D874).
// Shape: the comment list's inline constructor zeroes the head, allocates the 16-byte node, self-links it and stores it
// after the `add esp,4` (as the landed default constructor 0x00485C90); ErrorCallback and CursorPos are left to Init.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace N_0099a2d0 { class CBase {
public:
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
}; } using N_0099a2d0::CBase;

struct CCharStringData {
    char*         PData;                           // +0x0
    unsigned long Length;                          // +0x4
};
extern char g_emptyLiteral_0129aaf4[];             // 0x0129AAF4: ""

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString(const char* text, long length);    // 0x0099EBF0 (-1 = strlen)
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->PData; }
};

struct CStringParserCommentList {                  // retail list<CStringParserCommentBoundaries>: the head pointer only
    struct SNode { SNode* Next; SNode* Prev; char* PStart; char* PEnd; };
    SNode* m_pHead;
    CStringParserCommentList() : m_pHead(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->Next = node;
        node->Prev = node;
        m_pHead = node;
    }
    CStringParserCommentList(const CStringParserCommentList& other);   // 0x00463871
};

struct CCursorPos {
    bool          InQuotes;                        // +0x0
    unsigned long StringPos;                       // +0x4
    unsigned long Line;                            // +0x8
};

struct CParsedItem;
typedef void (*TStringParserErrorCallback)(CCharString* message);

class CStringParser : public CBase {               // vtable 0x0122D874
public:
    char*                      String;             // +0x04
    unsigned long              BaseLineOffset;     // +0x08
    CCharString                Filename;           // +0x0C
    CCharString                TempCharString;     // +0x10
    CStringParserCommentList   Comments;           // +0x14
    TStringParserErrorCallback ErrorCallback;      // +0x18
    CCursorPos                 CursorPos;          // +0x1C
    CStringParser(char* text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);
    void Init(char* text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530
};

CStringParser::CStringParser(char* text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset)
{
    Init(text, errorCallback, filename, baseLineOffset);
}