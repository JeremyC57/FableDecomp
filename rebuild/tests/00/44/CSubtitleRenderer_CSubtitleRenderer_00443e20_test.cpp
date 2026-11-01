// Test for CSubtitleRenderer::~CSubtitleRenderer (0x00443E20, compiler-generated): members are destroyed in reverse
// declaration order -- the fade helpers, the string/graphic/highlight/primitive vectors, the seven strings, the
// text-info queue, the text group tag -- and finally the CBaseClassNonCopyable base.
#include <cstdio>
static char g_log[32]; static int g_n = 0;
struct Logged { char Tag; explicit Logged(char t) : Tag(t) {} ~Logged() { g_log[g_n++] = Tag; } };
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { g_log[g_n++] = 'B'; } };
class CSubtitleRenderer : public CBaseClassNonCopyable { public:
    Logged TextGroupTag, TextInfoQueue, Strings, Primitives, Graphics, Highlights, WideStrings, FadeA, FadeB;
    CSubtitleRenderer() : TextGroupTag('T'), TextInfoQueue('Q'), Strings('S'), Primitives('P'), Graphics('G'),
                          Highlights('H'), WideStrings('W'), FadeA('a'), FadeB('b') {}
};
int main()
{
    CBaseClassNonCopyable* p = new CSubtitleRenderer;
    delete p;
    const char* want = "baWHGPSQTB";
    bool ok = g_n == 10;
    for (int i = 0; ok && i < 10; ++i) if (g_log[i] != want[i]) ok = false;
    std::printf(ok ? "00443e20_TEST PASS\n" : "00443e20_TEST FAIL\n");
    return ok ? 0 : 1;
}