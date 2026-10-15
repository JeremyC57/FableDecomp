// Behaviour test for 0x00492E4B: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model: the full SetText records what it was given; the overloads forward with converted / empty captions.
struct W { char s[32]; W() { s[0] = 0; } W(const char* t) { std::strcpy(s, t); } bool eq(const char* t) const { return std::strcmp(s, t) == 0; } };
struct C { char s[32]; C(const char* t) { std::strcpy(s, t); } W ToWideString() const { W w("w"); std::strcat(w.s, s); return w; } };
struct Font { int id; };
struct R {
    const Font* font; W text, b1, b2, b3, b4; unsigned int flags; float time; bool clickPast, pause;
    void SetText(const Font& f, const W& t, const W& a, const W& b, const W& c, const W& d, unsigned int fl, float tm, bool cp, bool pg)
    { font = &f; text = t; b1 = a; b2 = b; b3 = c; b4 = d; flags = fl; time = tm; clickPast = cp; pause = pg; }
    void SetText(const Font& f, const W& t, unsigned int fl, float tm, bool cp, bool pg)
    {
        W button4(""), button3(""), button2(""), button1("");
        SetText(f, t, button1, button2, button3, button4, fl, tm, cp, pg);
    }
};
static void Run()
{
    R r; Font f; r.b1 = W("old");
    r.SetText(f, W("Hi"), 7u, 2.5f, true, false);
    CHECK(r.font == &f && r.text.eq("Hi") && r.b1.eq("") && r.b2.eq("") && r.b3.eq("") && r.b4.eq(""));
    CHECK(r.flags == 7u && r.time == 2.5f && r.clickPast && !r.pause);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00492e4b_TEST FAIL\n"); return 1; }
    std::printf("00492e4b_TEST PASS\n");
    return 0;
}