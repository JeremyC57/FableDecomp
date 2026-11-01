// behaviour test: NScript::CV_BookCollectingScript::~CV_BookCollectingScript (implicit) -- BookOwned, BookDonated,
// BookDef, seh_Girl, seh_Boy, BookReactions are destroyed in that (reverse declaration) order, then the base.
#include <cstdio>
static char g_log[32]; static int g_n = 0;
struct Tracked { char tag; explicit Tracked(char t) : tag(t) { } ~Tracked() { g_log[g_n++] = tag; } };
class CScriptBase { public: virtual ~CScriptBase() { g_log[g_n++] = 'B'; } };
struct CLevelScriptBase : CScriptBase { };
struct CV_BookCollectingScript : CLevelScriptBase {
    bool ReadingBook; Tracked BookReactions; Tracked seh_Boy; Tracked seh_Girl; long GossipState;
    Tracked BookDef; Tracked BookDonated; Tracked BookOwned;
    CV_BookCollectingScript() : BookReactions('R'), seh_Boy('b'), seh_Girl('g'), BookDef('D'), BookDonated('d'), BookOwned('o') { }
};
int main()
{
    CScriptBase* p = new CV_BookCollectingScript;
    delete p;
    g_log[g_n] = 0;
    const char* want = "odDgbRB";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("00e57aa0_TEST FAIL\n"); return 1; }
    std::printf("00e57aa0_TEST PASS\n");
    return 0;
}