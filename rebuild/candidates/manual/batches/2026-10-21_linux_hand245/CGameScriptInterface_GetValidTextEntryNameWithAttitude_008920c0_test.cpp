// Behaviour test for 008920c0 CGameScriptInterface::GetValidTextEntryNameWithAttitude: walk the attitude graph until a text entry exists.
#include <cstdio>
#include <cstring>
static const char* g_names[4] = { "HATE", "LIKE", "LOVE", "NONE" };
static const char* g_exists[2];
static bool Exists(const char* s) { for (int i = 0; i < 2; ++i) if (g_exists[i] && !std::strcmp(g_exists[i], s)) return true; return false; }
static void Get(const char* base, int attitude, char* out)
{
    char name[64];
    std::sprintf(name, "%s_%s", base, g_names[attitude]);
    if (!Exists(name)) {
        do {
            attitude = attitude + 1;
            std::sprintf(name, "%s_%s", base, g_names[attitude]);
            if (!std::strcmp(g_names[attitude], "NONE")) { std::strcpy(name, base); break; }
        } while (!Exists(name));
    }
    std::strcpy(out, name);
}
int main()
{
    char out[64];
    g_exists[0] = "GREET_LOVE"; g_exists[1] = 0;
    Get("GREET", 0, out); bool a = !std::strcmp(out, "GREET_LOVE");
    Get("GREET", 2, out); bool b = !std::strcmp(out, "GREET_LOVE");
    g_exists[0] = 0;
    Get("GREET", 1, out); bool c = !std::strcmp(out, "GREET");
    if (!a || !b || !c) { std::printf("008920c0_TEST FAIL\n"); return 1; }
    std::printf("008920c0_TEST PASS\n");
    return 0;
}
