// CStringParser::Init(const CCharString&, void (*)(CCharString*), const CCharString&, long) -- keeps a copy of the text in TempCharString and initialises the parser over the text's characters
// Leaf: ?Init@CStringParser@@QAEXABVCCharString@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00403C20, 60 bytes (+4 int3 padding). Callees: CCharString::operator=
// (const CCharString&) @ 0x0099EFB0, CStringParser::Init(char*, callback, const CCharString&, long) @ 0x009B9530.
// Layout: retail CStringParser is 0x28 bytes, not the PDB's 0x30 (engine/CStringParser.h): the comment list is ONE word
// here (head pointer, 16-byte self-linked head node) where the PDB has 12 bytes, so ErrorCallback sits at +0x18 and
// CursorPos (InQuotes, StringPos, Line) at +0x1C. The header disagrees with the bytes, so the class is modelled locally
// with the PDB member names (CBase root with the vtable 0x0122D874).
// Shape: the characters come from the ARGUMENT (re-read through edi after the assignment), through the inline
// null-case-first accessor (the empty literal 0x0129AAF4 for a null string); the remaining arguments are forwarded.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CBase {
public:
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
};

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
    void Init(char* text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x009B9530
    void Init(const CCharString& text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);
};

void CStringParser::Init(const CCharString& text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset)
{
    TempCharString = text;
    Init(text.GetString(), errorCallback, filename, baseLineOffset);
}
