// Behaviour test for 0x004CDBD0: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of CSubtitledMessage: scalars, two strings and three tag maps (vector + Dirty flag).
struct Str { char s[32]; Str() { s[0] = 0; } Str(const char* t) { std::strcpy(s, t); } bool eq(const char* t) const { return std::strcmp(s, t) == 0; } };
struct TagVec { int n; int v[4]; TagVec() : n(0) {} };
struct TagMap { TagVec Vector; bool Dirty; TagMap() : Dirty(false) {} };
struct Bank { int id; };
static Bank g_banks[3] = { {0}, {1}, {2} };
static Bank* GetBank(int id) { return &g_banks[id]; }
struct Msg {
    Bank* PSpeechBank; int SpeechIndex; bool Speech3D; Str Text; Str Narrator; TagMap AttitudeTags, AnimationTags, CameraTags;
    Msg() : PSpeechBank(0), SpeechIndex(0), Speech3D(true), Narrator("UNKNOWN") {}
    Msg(const Str& text) : PSpeechBank(0), SpeechIndex(0), Speech3D(true), Text(text), Narrator("UNKNOWN") { PSpeechBank = GetBank(1); }
};
static void Run()
{
    Msg m(Str("Hello there"));
    CHECK(m.PSpeechBank == &g_banks[1] && m.SpeechIndex == 0 && m.Speech3D && m.Text.eq("Hello there") && m.Narrator.eq("UNKNOWN"));
    CHECK(m.AttitudeTags.Vector.n == 0 && !m.AttitudeTags.Dirty && !m.AnimationTags.Dirty && !m.CameraTags.Dirty);
}

int main()
{
    Run();
    if (g_fail) { std::printf("004cdbd0_TEST FAIL\n"); return 1; }
    std::printf("004cdbd0_TEST PASS\n");
    return 0;
}
