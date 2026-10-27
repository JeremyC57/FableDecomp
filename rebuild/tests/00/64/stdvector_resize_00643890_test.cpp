// 00643890 behaviour test for std::vector<CMinimapData::MinimapTheme>::resize(size_type, const MinimapTheme&)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
enum MinimapTheme { THEME_NONE = 0, THEME_GRASS = 3, THEME_WATER = 5 };
static int g_fillCalls = 0;
class ThemeVector {
public:
    MinimapTheme Storage[16];
    MinimapTheme* Start;
    MinimapTheme* Finish;
    ThemeVector() : Start(Storage), Finish(Storage) { }
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    MinimapTheme* begin() { return Start; }
    MinimapTheme* end() { return Finish; }
    void FillInsert(MinimapTheme* pos, unsigned int n, const MinimapTheme& x)   // stands in for _M_fill_insert 0x006436C0 (at end)
    {
        ++g_fillCalls;
        (void)pos;
        for (unsigned int i = 0; i < n; ++i)
            *Finish++ = x;
    }
    MinimapTheme* erase(MinimapTheme* first, MinimapTheme* last)
    {
        MinimapTheme* i = first;
        for (int n = (int)(Finish - last); n > 0; --n) {
            *i = *last;
            ++last;
            ++i;
        }
        Finish = i;
        return first;
    }
    void resize(unsigned int newSize, const MinimapTheme& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            FillInsert(end(), newSize - size(), x);
    }
};
static void run()
{
    ThemeVector v;
    MinimapTheme grass = THEME_GRASS, water = THEME_WATER;
    v.resize(4, grass);
    CHECK(v.size() == 4 && v.Start[3] == THEME_GRASS && g_fillCalls == 1);
    v.resize(6, water);
    CHECK(v.size() == 6 && v.Start[3] == THEME_GRASS && v.Start[5] == THEME_WATER);
    v.resize(2, water);
    CHECK(v.size() == 2 && v.Start[1] == THEME_GRASS && g_fillCalls == 2);
    v.resize(2, water);
    CHECK(v.size() == 2 && g_fillCalls == 3);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00643890_TEST FAIL\n");
        return 1;
    }
    std::printf("00643890_TEST PASS\n");
    return 0;
}