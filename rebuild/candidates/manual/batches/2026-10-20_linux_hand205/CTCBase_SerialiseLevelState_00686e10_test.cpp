// Behaviour test for CTCBase::SerialiseLevelState (0x00686E10).
#include <cstdio>
#include <cstdlib>

static int g_live = 0;
class CBaseClass { public: CBaseClass() { ++g_live; } virtual ~CBaseClass() { --g_live; } };
class CCharString { public: void* PStringData; CCharString() : PStringData(0) { ++g_live; } ~CCharString() { --g_live; } };
struct CStringParserCommentList {
    struct SNode { SNode* Next; SNode* Prev; char* PStart; char* PEnd; };
    SNode* m_pHead;
    CStringParserCommentList() : m_pHead(0)
    {
        SNode* node = (SNode*)std::malloc(sizeof(SNode));
        node->Next = node; node->Prev = node; m_pHead = node;
    }
    ~CStringParserCommentList() { std::free(m_pHead); }
};
class CStringParser : public CBaseClass {
public:
    char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
    CStringParserCommentList Comments;
    CStringParser() { String = 0; }
};
struct CPersistContext { unsigned char _body[0x2c]; };
static bool g_begin = true, g_shouldSave = true;
static int g_begins = 0, g_ends = 0, g_serialises = 0;
static bool g_parserOk = false, g_argSeen = false;
class CTCBase {
public:
    virtual void OnSerialise(CPersistContext&, bool arg) { ++g_serialises; g_argSeen = arg; }
    virtual bool ShouldBeSaved() { return g_shouldSave; }
    bool BeginSerialising(bool, CCharString&, CStringParser& parser, CPersistContext&)
    { ++g_begins; g_parserOk = parser.String == 0 && parser.Comments.m_pHead->Next == parser.Comments.m_pHead; return g_begin; }
    void EndSerialising(bool, CCharString&) { ++g_ends; }
    void SerialiseLevelState(bool saving, CCharString& data, bool arg)
    {
        if (saving && !ShouldBeSaved())
            return;
        CStringParser parser;
        CPersistContext context;
        if (BeginSerialising(saving, data, parser, context)) {
            OnSerialise(context, arg);
            EndSerialising(saving, data);
        }
    }
};

int main()
{
    CTCBase tc; CCharString data;
    tc.SerialiseLevelState(true, data, true);
    bool ok = g_begins == 1 && g_serialises == 1 && g_ends == 1 && g_parserOk && g_argSeen && g_live == 1;
    g_shouldSave = false;
    tc.SerialiseLevelState(true, data, false);
    ok = ok && g_begins == 1;
    tc.SerialiseLevelState(false, data, false);           // loading ignores the save test
    ok = ok && g_begins == 2 && g_serialises == 2 && !g_argSeen;
    g_begin = false;
    tc.SerialiseLevelState(false, data, true);
    ok = ok && g_begins == 3 && g_serialises == 2 && g_ends == 2 && g_live == 1;
    if (!ok) { std::printf("00686e10_TEST FAIL\n"); return 1; }
    std::printf("00686e10_TEST PASS\n");
    return 0;
}
