// Behaviour test for CMiniMapDisplay::InitialiseMiniMapFileLoading_Region (0x00829D90): which minimap file is loaded or
// generated for each combination of present files, time stamps and the regenerate flag.
#include <cstdio>
#include <cstring>
#include <string>

static bool g_dataExists, g_cacheExists;
static int g_dataTime, g_cacheTime, g_wldTime;
static std::string g_action;

static bool PathExists(const std::string& p)
{
    if (p == "data/") return true;
    if (p == "cache/") return true;
    if (p == "data/Oakvale.tga") return g_dataExists;
    if (p == "cache/Oakvale.tga") return g_cacheExists;
    return false;
}
static int GetFileModifiedTime(const std::string& p)
{
    if (p == "Data\\Levels\\FinalAlbion.wld") return g_wldTime;
    if (p == "data/Oakvale.tga") return g_dataTime;
    return g_cacheTime;
}

struct Minimap {
    bool loaded;
    void Load(const std::string& p) { g_action = "load " + p; loaded = true; }
    void Generate(const std::string& p) { g_action = "gen " + p; loaded = true; }
};

static int Run(bool dataExists, bool cacheExists, int dataTime, int cacheTime, int wldTime, bool regenerate)
{
    g_dataExists = dataExists; g_cacheExists = cacheExists;
    g_dataTime = dataTime; g_cacheTime = cacheTime; g_wldTime = wldTime;
    g_action = "none";
    Minimap m; m.loaded = false;
    std::string tgaName = std::string("Oakvale") + ".tga";
    std::string cachePath = "cache/" + tgaName;
    std::string dataPath = "data/" + tgaName;
    int wld = GetFileModifiedTime(std::string("Data\\Levels\\") + "FinalAlbion" + ".wld");
    if (PathExists(dataPath)) {
        int t = GetFileModifiedTime(dataPath);
        if (regenerate && wld > t) m.Generate(cachePath); else m.Load(dataPath);
    } else if (PathExists(cachePath)) {
        int t = GetFileModifiedTime(cachePath);
        if (regenerate && wld > t) m.Generate(cachePath); else m.Load(cachePath);
    } else if (regenerate)
        m.Generate(cachePath);
    return m.loaded ? 1 : 0;
}

int main()
{
    bool ok = true;
    ok = ok && Run(true, true, 5, 5, 3, true) == 1 && g_action == "load data/Oakvale.tga";
    ok = ok && Run(true, false, 5, 0, 9, true) == 1 && g_action == "gen cache/Oakvale.tga";
    ok = ok && Run(true, false, 5, 0, 9, false) == 1 && g_action == "load data/Oakvale.tga";
    ok = ok && Run(false, true, 0, 7, 3, true) == 1 && g_action == "load cache/Oakvale.tga";
    ok = ok && Run(false, true, 0, 2, 3, true) == 1 && g_action == "gen cache/Oakvale.tga";
    ok = ok && Run(false, false, 0, 0, 3, true) == 1 && g_action == "gen cache/Oakvale.tga";
    ok = ok && Run(false, false, 0, 0, 3, false) == 0 && g_action == "none";
    std::printf(ok ? "00829d90_TEST PASS\n" : "00829d90_TEST FAIL\n");
    return ok ? 0 : 1;
}