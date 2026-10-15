// Behaviour test for 0x00719E80: the reconstructed logic over a host model.
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
    Msg& operator=(const Msg& o)
    {
        PSpeechBank = o.PSpeechBank; SpeechIndex = o.SpeechIndex; Speech3D = o.Speech3D; Text = o.Text; Narrator = o.Narrator;
        AttitudeTags = o.AttitudeTags; AnimationTags = o.AnimationTags; CameraTags = o.CameraTags;
        return *this;
    }
};
static Msg Make()
{
    Msg m; m.PSpeechBank = &g_banks[2]; m.SpeechIndex = 9; m.Speech3D = false; m.Text = Str("t"); m.Narrator = Str("Guild");
    m.AttitudeTags.Vector.n = 1; m.AttitudeTags.Vector.v[0] = 5; m.AttitudeTags.Dirty = true;
    m.CameraTags.Vector.n = 2; m.CameraTags.Dirty = false; m.AnimationTags.Dirty = true;
    return m;
}
static void Same(const Msg& a, const Msg& b)
{
    CHECK(a.PSpeechBank == b.PSpeechBank && a.SpeechIndex == b.SpeechIndex && a.Speech3D == b.Speech3D && a.Text.eq(b.Text.s) && a.Narrator.eq(b.Narrator.s));
    CHECK(a.AttitudeTags.Vector.n == b.AttitudeTags.Vector.n && a.AttitudeTags.Vector.v[0] == b.AttitudeTags.Vector.v[0] && a.AttitudeTags.Dirty == b.AttitudeTags.Dirty);
    CHECK(a.AnimationTags.Dirty == b.AnimationTags.Dirty && a.CameraTags.Vector.n == b.CameraTags.Vector.n && a.CameraTags.Dirty == b.CameraTags.Dirty);
}
static void Run()
{
    Msg a = Make();
    Msg b;
    Msg& r = (b = a);
    CHECK(&r == &b);
    Same(a, b);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00719e80_TEST FAIL\n"); return 1; }
    std::printf("00719e80_TEST PASS\n");
    return 0;
}
