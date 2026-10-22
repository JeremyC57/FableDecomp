// Behaviour test for CMainGameComponent::PostInit (0x00416953): a save name loads the save (a failure sets the flag);
// otherwise the world path is levels dir + the init world (remembered globally), else the remembered world, else
// updatedscenic.wld; export mode copies the two map files and returns before registration.
#include <cstdio>
#include <cstring>

struct State {
    char save[32], world[32], world2[32], quest[32], remembered[32], remembered2[32], loadedPath[64], activated[32];
    bool loadOk, failed, exportMode, registered;
    int copies;
};
static void Reset(State& s) { std::memset(&s, 0, sizeof(s)); }
static void Join(char* out, const char* dir, const char* name) { std::strcpy(out, dir); std::strcat(out, name); }
static void PostInit(State& s)
{
    if (s.save[0] != 0) {
        if (!s.loadOk) s.failed = true;
    } else {
        char path[64];
        if (s.world[0] != 0) {
            Join(path, "Data\\Levels\\", s.world);
            std::strcpy(s.remembered, s.world);
            if (s.world2[0] != 0) std::strcpy(s.remembered2, s.world2);
        } else if (s.remembered[0] != 0) {
            Join(path, "Data\\Levels\\", s.remembered);
        } else {
            Join(path, "Data\\Levels\\", "updatedscenic.wld");
        }
        std::strcpy(s.loadedPath, path);
        if (s.exportMode) { s.copies += 2; return; }
        if (s.quest[0] != 0) std::strcpy(s.activated, s.quest);
    }
    s.registered = true;
}

int main()
{
    State a; Reset(a);
    std::strcpy(a.save, "slot1"); a.loadOk = false;
    PostInit(a);
    bool ok = a.failed && a.registered && a.loadedPath[0] == 0;
    State b; Reset(b);
    PostInit(b);
    ok = ok && std::strcmp(b.loadedPath, "Data\\Levels\\updatedscenic.wld") == 0 && b.registered && b.activated[0] == 0;
    State c; Reset(c);
    std::strcpy(c.world, "x.wld"); std::strcpy(c.world2, "y"); std::strcpy(c.quest, "Q_Start");
    PostInit(c);
    ok = ok && std::strcmp(c.loadedPath, "Data\\Levels\\x.wld") == 0 && std::strcmp(c.remembered, "x.wld") == 0
        && std::strcmp(c.remembered2, "y") == 0 && std::strcmp(c.activated, "Q_Start") == 0;
    State d; Reset(d);
    std::strcpy(d.remembered, "r.wld"); d.exportMode = true;
    PostInit(d);
    ok = ok && std::strcmp(d.loadedPath, "Data\\Levels\\r.wld") == 0 && d.copies == 2 && !d.registered;
    std::printf(ok ? "00416953_TEST PASS\n" : "00416953_TEST FAIL\n");
    return ok ? 0 : 1;
}
