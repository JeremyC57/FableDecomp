// Behaviour test for 0x00438020: CScreenMessage ctor: copies text/timings, LinesInText = 1 + newlines that are not the last character
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Msg {
    const char* text; long frames, chars, lines;
    Msg(const char* t, long f, long c) : text(t), frames(f), chars(c), lines(1)
    {
        long len = (long)std::strlen(t);
        for (long i = 0; i < len; ++i)
            if (t[i] == '\n' && i < len - 1) ++lines;
    }
};
static void Run()
{
    Msg a("one", 10, 2); CHECK(a.lines == 1 && a.frames == 10 && a.chars == 2);
    Msg b("one\ntwo\nthree", 1, 0); CHECK(b.lines == 3);
    Msg c("trailing\n", 1, 0); CHECK(c.lines == 1);
    Msg d("", 1, 0); CHECK(d.lines == 1);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00438020_TEST FAIL\n"); return 1; }
    std::printf("00438020_TEST PASS\n");
    return 0;
}