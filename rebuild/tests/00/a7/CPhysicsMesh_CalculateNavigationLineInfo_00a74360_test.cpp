#include <cstdio>
static int g_cleared = 0, g_processed = 0;
struct LineArr { int n; LineArr() : n(0) {} void clear() { ++g_cleared; n = 0; } };
struct Mesh {
    long levelsFound; bool NavigationLineListInfo; LineArr lines[16]; long count;
    long Levels() const { return levelsFound; }
    void resize(long n, const LineArr&) { count = n; }
    void Process() { ++g_processed; }
    void Calc()
    {
        long levels = Levels();
        if (levels != 0) NavigationLineListInfo = true;
        if (levels < 1) levels = 1;
        resize(levels, LineArr());
        for (long i = 0; i < count; ++i) lines[i].clear();
        Process();
    }
};
int main()
{
    Mesh m; m.levelsFound = 0; m.NavigationLineListInfo = false; m.Calc();
    int ok = !m.NavigationLineListInfo && m.count == 1 && g_cleared == 1 && g_processed == 1;
    m.levelsFound = 4; m.Calc();
    ok = ok && m.NavigationLineListInfo && m.count == 4 && g_cleared == 5;
    if (!ok) { std::printf("00a74360_TEST FAIL\n"); return 1; }
    std::printf("00a74360_TEST PASS\n");
    return 0;
}