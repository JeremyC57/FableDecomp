#include <cstdio>
#include <cstdlib>
struct CDataBank;
struct CASoundManager;
static int g_wideStrings = 0;
class CWideString { public: void* PStringData; CWideString() : PStringData(0) { ++g_wideStrings; } };
struct C2DBoxF { float X0, Y0, X1, Y1; C2DBoxF() { } };
struct SSubTitleNode { SSubTitleNode* Next; SSubTitleNode* Prev; unsigned char SubTitle[0x1c]; };
struct CRetailList_SubTitle {
    SSubTitleNode* Node;
    CRetailList_SubTitle() : Node(0)
    {
        SSubTitleNode* sentinel = static_cast<SSubTitleNode*>(std::malloc(sizeof(SSubTitleNode)));
        sentinel->Next = sentinel; sentinel->Prev = sentinel; Node = sentinel;
    }
};
struct CMovieSubtitles {
    CRetailList_SubTitle SubTitles; CDataBank* pTextBank; long Fps; CASoundManager* PSoundManager; bool IsSubtitleTime;
    CWideString FormattedText; C2DBoxF VSubtitleWindow; long FrameCounter;
    CMovieSubtitles() : SubTitles(), pTextBank(0), Fps(30), IsSubtitleTime(false), FormattedText(), VSubtitleWindow(), FrameCounter(0) { }
};
int main()
{
    CMovieSubtitles* m = new CMovieSubtitles;
    SSubTitleNode* s = m->SubTitles.Node;
    if (s == 0 || s->Next != s || s->Prev != s || m->pTextBank != 0 || m->Fps != 30 || m->IsSubtitleTime || g_wideStrings != 1
        || m->FormattedText.PStringData != 0 || m->FrameCounter != 0) { std::printf("009bdba0_TEST FAIL\n"); return 1; }
    std::free(s);
    delete m;
    std::printf("009bdba0_TEST PASS\n");
    return 0;
}