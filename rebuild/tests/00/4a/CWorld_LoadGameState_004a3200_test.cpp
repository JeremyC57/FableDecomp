// Behaviour test for CWorld::LoadGameState (0x004A3200): resolve the save path, then load now or queue it.
#include <cstdio>
#include <string>
static bool g_anyDir; static std::string g_profilePath, g_found, g_searched; static int g_exists, g_isFile, g_loadOk, g_heroOk;
static std::string g_loadedPath; static int g_loads;
struct World {
    long status; std::string pending; bool aboutToLoad;
    bool LoadInternal(const std::string& p) { g_loadedPath = p; ++g_loads; return g_loadOk != 0; }
    bool Hero(const std::string&) { return g_heroOk != 0; }
    bool Load(const std::string& name, bool now)
    {
        if (status == 0 || now) {
            bool found = false;
            std::string path = g_profilePath;
            if (path != "") found = true;
            if (g_anyDir) {
                if (!found) return false;
                if (now) { aboutToLoad = true; bool ok = LoadInternal(path); aboutToLoad = false; if (!ok) return false; return Hero(name); }
                status = 1; pending = path; return true;
            }
            if (found && g_exists && g_isFile) {
                if (now) { aboutToLoad = true; bool ok = LoadInternal(path); aboutToLoad = false; if (!ok) return false; return Hero(name); }
                status = 1; pending = path; return true;
            }
            std::string full;
            if (g_found != "") full = g_found; else full = g_searched;
            if (full != "") {
                if (now) { aboutToLoad = true; bool ok = LoadInternal(path); aboutToLoad = false; if (!ok) return false; return Hero(name); }
                status = 1; pending = full; return true;
            }
        }
        return false;
    }
};
int main()
{
    World w; w.status = 0; w.aboutToLoad = false;
    g_profilePath = "P"; g_exists = 1; g_isFile = 1; g_loadOk = 1; g_heroOk = 1;
    int ok = w.Load("n", false) && w.status == 1 && w.pending == "P";
    ok = ok && !w.Load("n", false);                 // already pending
    ok = ok && w.Load("n", true) && g_loadedPath == "P";
    g_heroOk = 0; ok = ok && !w.Load("n", true);
    World v; v.status = 0; v.aboutToLoad = false; g_isFile = 0; g_found = ""; g_searched = "U";
    ok = ok && v.Load("n", false) && v.pending == "U";
    g_searched = ""; World u; u.status = 0; ok = ok && !u.Load("n", false);
    g_anyDir = true; g_profilePath = ""; ok = ok && !u.Load("n", true);
    if (!ok) { std::printf("004a3200_TEST FAIL\n"); return 1; }
    std::printf("004a3200_TEST PASS\n");
    return 0;
}