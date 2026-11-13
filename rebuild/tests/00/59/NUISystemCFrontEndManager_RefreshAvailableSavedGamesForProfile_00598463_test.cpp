// Behaviour test for NUISystem::CFrontEndManager::RefreshAvailableSavedGamesForProfile (0x00598463): a host model of the row's
// statements (src/00598463.cpp) over a fake profile manager, file system and component tree. Checks: the empty slot is skipped
// (unless the profile is a debug one), each kept save gets a button at y = 0, 30, 60 ... in list order, the button action is
// UI_ACTION_TYPE_LOAD_GAME (17) when the save verifies and its ".hs" header is absent or verifies, else
// UI_ACTION_TYPE_NEXT_SCREEN_INVALID_SAVE (220); NumberSaveGames counts every listed save plus every existing header; the old
// save list is marked UI_DELETE_THIS_INSTANCE, the title gets the profile name, and the function returns 0.
#include <cstdio>
#include <cstring>

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

struct Save { const char* name; bool valid; bool header_exists; bool header_valid; };
struct Button { char name[32]; float y; long action_type; char param[32]; unsigned long index; };

struct World {
    Save saves[8]; int save_count;
    bool debug_profile;
    bool old_list_present; long old_list_deletion;
    char title[64];
    Button buttons[8]; int button_count;
    int children_added; int file_opens;
};
static World W;
static const char* kEmptySlot = "EMPTY";

static bool VerifySignatureCached(const char* path)
{
    for (int i = 0; i < W.save_count; ++i) {
        char hs[64]; std::sprintf(hs, "%s.hs", W.saves[i].name);
        if (std::strcmp(path, W.saves[i].name) == 0) return W.saves[i].valid;
        if (std::strcmp(path, hs) == 0) return W.saves[i].header_valid;
    }
    return false;
}
static bool PathExists(const char* path)
{
    for (int i = 0; i < W.save_count; ++i) {
        char hs[64]; std::sprintf(hs, "%s.hs", W.saves[i].name);
        if (std::strcmp(path, hs) == 0) return W.saves[i].header_exists;
    }
    return false;
}

struct Manager { unsigned long NumberSaveGames; };

static long Refresh(Manager& m)
{
    m.NumberSaveGames = 0;
    std::sprintf(W.title, "Load game: %s", "Hero");                       // GetTextBySymbol + ConstructString(profile name)
    if (W.old_list_present) W.old_list_deletion = 1;                      // CDeletion::UI_DELETE_THIS_INSTANCE
    W.children_added += 3;                                               // list, text area, screenshot viewport
    float y = 0.0f;
    for (unsigned long i = 0; i < (unsigned long)W.save_count; ++i) {
        const char* name = W.saves[i].name;
        if (!W.debug_profile && std::strcmp(name, kEmptySlot) == 0)
            continue;
        Button& b = W.buttons[W.button_count++];
        std::strcpy(b.name, name); b.y = y; b.index = i;
        bool save_valid = VerifySignatureCached(name);
        if (save_valid) ++W.file_opens;
        char header_name[64]; std::sprintf(header_name, "%s.hs", name);
        bool header_exists = false;
        bool header_valid = VerifySignatureCached(header_name);
        if (PathExists(header_name)) {
            header_exists = true;
            if (header_valid) ++W.file_opens;
            m.NumberSaveGames++;
        }
        if (save_valid && (!header_exists || header_valid)) b.action_type = 17;
        else b.action_type = 220;
        std::strcpy(b.param, name);
        y += 30.0f;
        m.NumberSaveGames++;
    }
    return 0;
}

int main()
{
    std::memset(&W, 0, sizeof W);
    Save s[5] = {
        { "Save1", true,  false, false },   // valid, no header  -> load
        { "EMPTY", true,  false, false },   // empty slot        -> skipped
        { "Save2", true,  true,  true  },   // valid + header ok -> load
        { "Save3", true,  true,  false },   // header corrupt    -> invalid
        { "Save4", false, false, false },   // save corrupt      -> invalid
    };
    std::memcpy(W.saves, s, sizeof s); W.save_count = 5; W.old_list_present = true;
    Manager m; m.NumberSaveGames = 99;
    long r = Refresh(m);
    CHECK(r == 0);
    CHECK(W.button_count == 4);
    CHECK(std::strcmp(W.buttons[0].name, "Save1") == 0 && W.buttons[0].action_type == 17 && W.buttons[0].y == 0.0f);
    CHECK(std::strcmp(W.buttons[1].name, "Save2") == 0 && W.buttons[1].action_type == 17 && W.buttons[1].y == 30.0f && W.buttons[1].index == 2);
    CHECK(std::strcmp(W.buttons[2].name, "Save3") == 0 && W.buttons[2].action_type == 220 && W.buttons[2].y == 60.0f);
    CHECK(std::strcmp(W.buttons[3].name, "Save4") == 0 && W.buttons[3].action_type == 220 && W.buttons[3].y == 90.0f);
    CHECK(m.NumberSaveGames == 4 + 2);
    CHECK(W.old_list_deletion == 1 && W.children_added == 3);
    CHECK(W.file_opens == 3 + 1);

    // a debug profile lists the empty slot too
    std::memset(&W, 0, sizeof W);
    std::memcpy(W.saves, s, sizeof s); W.save_count = 5; W.debug_profile = true;
    Refresh(m);
    CHECK(W.button_count == 5 && std::strcmp(W.buttons[1].name, "EMPTY") == 0);

    if (g_fail) { std::printf("00598463_TEST FAIL\n"); return 1; }
    std::printf("00598463_TEST PASS\n");
    return 0;
}