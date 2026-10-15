// Behaviour test for 0x00493049: the reconstructed logic over a host model.
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
    void SetText(const Font& f, const C& t, const C& a, const C& b, const C& c, const C& d, unsigned int fl, float tm, bool cp, bool pg)
    {
        SetText(f, t.ToWideString(), a.ToWideString(), b.ToWideString(), c.ToWideString(), d.ToWideString(), fl, tm, cp, pg);
    }
};
static void Run()
{
    R r; Font f;
    r.SetText(f, C("T"), C("1"), C("2"), C("3"), C("4"), 3u, 1.0f, false, true);
    CHECK(r.text.eq("wT") && r.b1.eq("w1") && r.b2.eq("w2") && r.b3.eq("w3") && r.b4.eq("w4"));
    CHECK(r.flags == 3u && r.time == 1.0f && !r.clickPast && r.pause);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00493049_TEST FAIL\n"); return 1; }
    std::printf("00493049_TEST PASS\n");
    return 0;
}