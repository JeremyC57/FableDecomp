#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 00403c20: CStringParser on the retail layout (comment list = head pointer, cursor = InQuotes,
// StringPos, Line), with recording stand-ins for the out-of-line callees.
static char g_emptyLiteral[1] = { 0 };
class CCharStringData { public: char* PData; unsigned long Length; };
static int g_stringCopies = 0;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString() : PStringData(0) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_stringCopies; }
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
    char* GetString() const { return PStringData == 0 ? g_emptyLiteral : PStringData->PData; }
};
static int g_listCopies = 0;
struct CStringParserCommentList {
    struct SNode { SNode* Next; SNode* Prev; char* PStart; char* PEnd; };
    SNode* m_pHead;
    CStringParserCommentList() : m_pHead(0)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        node->Next = node;
        node->Prev = node;
        m_pHead = node;
    }
    CStringParserCommentList(const CStringParserCommentList& other) : m_pHead(other.m_pHead) { ++g_listCopies; }
};
struct CCursorPos {
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
class CParsedItem { public: long Type; long Value; };
typedef void (*TStringParserErrorCallback)(CCharString* message);
class CBase { public: CBase() {} virtual ~CBase() {} };
static void ErrorSink(CCharString* message) { (void)message; }
class CStringParser : public CBase {
public:
    char*                      String;
    unsigned long              BaseLineOffset;
    CCharString                Filename;
    CCharString                TempCharString;
    CStringParserCommentList   Comments;
    TStringParserErrorCallback ErrorCallback;
    CCursorPos                 CursorPos;
    void Init(char* text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset)
    {
        String = text; ErrorCallback = errorCallback; Filename = filename; BaseLineOffset = baseLineOffset;
        CursorPos.InQuotes = false; CursorPos.StringPos = 0; CursorPos.Line = 1;
    }
    void Init(const CCharString& text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);
};
void CStringParser::Init(const CCharString& text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset)
{
    TempCharString = text;
    Init(text.GetString(), errorCallback, filename, baseLineOffset);
}
#define FAIL do { std::printf("00403c20_TEST FAIL\n"); return 1; } while (0)
int main()
{
    char buffer[] = "alpha beta";
    CCharStringData data = { buffer, 10 };
    CCharString text; text.PStringData = &data;
    CCharString file; CCharStringData fileData = { buffer, 5 }; file.PStringData = &fileData;
    CStringParser p; p.String = 0; p.BaseLineOffset = 0; p.ErrorCallback = 0;
    p.Init(text, ErrorSink, file, 12);
    if (p.TempCharString.PStringData != &data || p.String != buffer || p.ErrorCallback != ErrorSink || p.Filename.PStringData != &fileData
        || p.BaseLineOffset != 12 || p.CursorPos.StringPos != 0) FAIL;
    CCharString empty;
    p.Init(empty, 0, file, 3);
    if (p.String != g_emptyLiteral || p.TempCharString.PStringData != 0 || p.BaseLineOffset != 3) FAIL;
    std::printf("00403c20_TEST PASS\n");
    return 0;
}