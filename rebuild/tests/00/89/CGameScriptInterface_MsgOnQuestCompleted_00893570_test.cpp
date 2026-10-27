#include <cstdio>
#include <cstring>
// Behaviour model: true only when a message exists and its quest name equals the argument.
static bool eq(const char* a, const char* b) { if (a == b) return true; if (!a || !b) return false; if (std::strlen(a) != std::strlen(b)) return false; return std::strcmp(a, b) == 0; }
static bool on(const char* questOfMessage, const char* name) { return questOfMessage && eq(questOfMessage, name); }
int main()
{
    if (!on("Q_ARENA", "Q_ARENA") || on("Q_ARENA", "Q_ARENB") || on(0, "Q_ARENA") || on("Q_A", "Q_ARENA")) { std::printf("00893570_TEST FAIL\n"); return 1; }
    std::printf("00893570_TEST PASS\n");
    return 0;
}