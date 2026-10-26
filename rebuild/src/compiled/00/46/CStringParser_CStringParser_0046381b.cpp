// CStringParser::CStringParser(const CStringParser&) -- the implicit copy constructor: copies the text pointer, base line, both strings, the comment list, the error callback and the cursor position
// Leaf: ??0CStringParser
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046381B, 86 bytes. Callees: CCharString::CCharString(const CCharString&)
// @ 0x0099EC30 (twice), the comment list's copy constructor @ 0x00463871 (manifest "CEngineSceneGrid_InitFromCell" is
// propagated).
// Layout: retail CStringParser is 0x28 bytes, not the PDB's 0x30 (engine/CStringParser.h): the comment list is ONE word
// here (head pointer, 16-byte self-linked head node) where the PDB has 12 bytes, so ErrorCallback sits at +0x18 and
// CursorPos (InQuotes, StringPos, Line) at +0x1C. The header disagrees with the bytes, so the class is modelled locally
// with the PDB member names (CBase root with the vtable 0x0122D874).
// Shape: CBase's copy is inline and empty (its dead vtable store dropped), so the derived vtable is stored first; the
// scalar words all go through eax and the 12-byte POD CCursorPos is a `movsd` block copy, which is the size-optimised
// TU (odd address): #pragma optimize("s", on). Written as the explicit memberwise copy constructor.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CBase {
public:
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();
};

class CCharStringData { public:
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

class CParsedItem;
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
    CStringParser(const CStringParser& other);
};

CStringParser::CStringParser(const CStringParser& other)
    : CBase(other),
      String(other.String),
      BaseLineOffset(other.BaseLineOffset),
      Filename(other.Filename),
      TempCharString(other.TempCharString),
      Comments(other.Comments),
      ErrorCallback(other.ErrorCallback),
      CursorPos(other.CursorPos)
{
}