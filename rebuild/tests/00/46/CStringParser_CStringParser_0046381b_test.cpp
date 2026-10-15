#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0046381b: CStringParser on the retail layout (comment list = head pointer, cursor = InQuotes,
// StringPos, Line), with recording stand-ins for the out-of-line callees.
static char g_emptyLiteral[1] = { 0 };
struct CCharStringData { char* PData; unsigned long Length; };
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
struct CParsedItem { long Type; long Value; };
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
    CStringParser() {}
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
#define FAIL do { std::printf("0046381b_TEST FAIL\n"); return 1; } while (0)
int main()
{
    char buffer[] = "abc";
    CCharStringData d1 = { buffer, 3 }; CCharStringData d2 = { buffer, 1 };
    CStringParser a; a.String = buffer; a.BaseLineOffset = 9; a.Filename.PStringData = &d1; a.TempCharString.PStringData = &d2;
    a.ErrorCallback = ErrorSink; a.CursorPos.InQuotes = true; a.CursorPos.StringPos = 2; a.CursorPos.Line = 7;
    CStringParser b(a);
    if (b.String != buffer || b.BaseLineOffset != 9 || b.Filename.PStringData != &d1 || b.TempCharString.PStringData != &d2
        || b.Comments.m_pHead != a.Comments.m_pHead || b.ErrorCallback != ErrorSink || !b.CursorPos.InQuotes || b.CursorPos.StringPos != 2
        || b.CursorPos.Line != 7 || g_stringCopies != 2 || g_listCopies != 1) FAIL;
    free(a.Comments.m_pHead);
    std::printf("0046381b_TEST PASS\n");
    return 0;
}