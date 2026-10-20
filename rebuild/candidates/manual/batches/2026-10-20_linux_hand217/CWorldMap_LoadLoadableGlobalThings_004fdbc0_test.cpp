// Behaviour test for CWorldMap::LoadLoadableGlobalThings 0x004FDBC0: progress i/(n+1) for maps 1..n-1, load used proximity maps.
#include <cstdio>
#include <cstring>

struct CCharString {
    const char* Text;
    CCharString(const char* text, long = -1) : Text(text) { }
    ~CCharString() { }
};
static int g_progressCalls = 0; static float g_lastProgress = -1.0f; static bool g_textOk = true;
static void DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    ++g_progressCalls; g_lastProgress = progress;
    if (std::strcmp(text.Text, "Loading global things") || a || b) g_textOk = false;
}
struct CMapInfo { bool Used; bool LoadedOnPlayerProximity; };
template <class T> struct CArray {
    T* Start; T* Finish; T* EndOfStorage;
    unsigned long size() const { return (unsigned long)(Finish - Start); }
    const T& operator[](unsigned long i) const { return Start[i]; }
};
static long g_loaded[8]; static int g_loadCount = 0;
struct CWorldMap {
    CArray<CMapInfo> Maps;
    void LoadLoadableGlobalThingsForLevel(long level) { g_loaded[g_loadCount++] = level; }
    void LoadLoadableGlobalThings();
};

void CWorldMap::LoadLoadableGlobalThings()
{
    for (long i = 1; i < (long)Maps.size(); ++i)
    {
        DisplayProgress("Loading global things", (float)i / (Maps.size() + 1), false, false);
        const CMapInfo& map = Maps[i];
        if (map.Used && map.LoadedOnPlayerProximity)
            LoadLoadableGlobalThingsForLevel(i);
    }
}

int main()
{
    CMapInfo maps[4] = { { true, true }, { true, true }, { true, false }, { true, true } };
    CWorldMap w; w.Maps.Start = maps; w.Maps.Finish = maps + 4; w.Maps.EndOfStorage = maps + 4;
    bool ok = true;
    w.LoadLoadableGlobalThings();
    if (g_progressCalls != 3 || !g_textOk || g_lastProgress != 3.0f / 5.0f) ok = false;
    if (g_loadCount != 2 || g_loaded[0] != 1 || g_loaded[1] != 3) ok = false;
    w.Maps.Finish = maps + 1; g_progressCalls = 0; w.LoadLoadableGlobalThings();
    if (g_progressCalls != 0 || g_loadCount != 2) ok = false;
    if (!ok) { std::printf("004fdbc0_TEST FAIL\n"); return 1; }
    std::printf("004fdbc0_TEST PASS\n");
    return 0;
}
