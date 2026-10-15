#include <cstdio>
#include <cstdlib>
// Behaviour test for CStringParser::CStringParser(const CCharString&, callback, const CCharString&, long) (0x00404720).
struct CCharStringData { char* PData; unsigned long Length; };
char g_emptyLiteral_0129aaf4[] = "";
struct CCharString {
    CCharStringData* PStringData;
    CCharString() : PStringData(0) {}
    CCharString& operator=(const CCharString& o) { PStringData = o.PStringData; return *this; }
    char* GetString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->PData; }
};
struct SCommentNode { SCommentNode* Next; SCommentNode* Prev; long Start; long End; };
struct CCommentList {
    SCommentNode* Node;
    CCommentList() : Node(0) { SCommentNode* n = (SCommentNode*)std::malloc(sizeof(SCommentNode)); n->Next = n; n->Prev = n; Node = n; }
};
typedef void (*StringParserErrorCallback)(CCharString*);
static const char* g_initText = 0; static long g_initOffset = -1; static StringParserErrorCallback g_initCb = 0; static const CCharString* g_initFile = 0;
static void OnError(CCharString*) {}
struct CStringParser {
    virtual ~CStringParser() {}
    char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString; CCommentList Comments;
    StringParserErrorCallback ErrorCallback; long CursorPos[3];
    void Init(char* text, StringParserErrorCallback cb, const CCharString& filename, long baseLineOffset)
    { g_initText = text; g_initCb = cb; g_initFile = &filename; g_initOffset = baseLineOffset; }
    CStringParser(const CCharString& text, StringParserErrorCallback cb, const CCharString& filename, long baseLineOffset)
        : Filename(), TempCharString(), Comments()
    { TempCharString = text; Init(text.GetString(), cb, filename, baseLineOffset); }
};
int main()
{
    char chars[] = "hello"; CCharStringData data = { chars, 5 };
    CCharString text; text.PStringData = &data; CCharString file;
    CStringParser p(text, OnError, file, 7);
    int ok = p.TempCharString.PStringData == &data && g_initText == chars && g_initCb == OnError && g_initFile == &file && g_initOffset == 7
          && p.Comments.Node != 0 && p.Comments.Node->Next == p.Comments.Node && p.Comments.Node->Prev == p.Comments.Node && p.Filename.PStringData == 0;
    CCharString empty; CStringParser q(empty, OnError, file, 0);
    if (g_initText != g_emptyLiteral_0129aaf4) ok = 0;
    std::printf(ok ? "00404720_TEST PASS\n" : "00404720_TEST FAIL\n"); return ok ? 0 : 1;
}
