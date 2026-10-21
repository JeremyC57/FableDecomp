// Behaviour test for CTCBuyableHouse::SetupDressingAtLevel (0x006C0330): show "Dress<digits>" things listing the level.
#include <cstdio>
#include <cstring>
#include <cctype>
struct Thing { const char* name; bool dead; int limbo; };
static int nicmp(const char* a, const char* b, unsigned n)
{
    for (unsigned i = 0; i < n; ++i) {
        int x = std::tolower((unsigned char)a[i]), y = std::tolower((unsigned char)b[i]);
        if (x != y) return x - y;
        if (!x) return 0;
    }
    return 0;
}
struct House {
    long DressLevel; Thing* things[4]; int n;
    void SetupDressingAtLevel(long level)
    {
        DressLevel = level;
        const char* prefix = "Dress"; long plen = 5;
        for (int k = 0; k < n; ++k) {
            Thing* t = things[k];
            if (t && !t->dead && nicmp(t->name, prefix, plen) == 0) {
                if (level == -1) t->limbo = 0;
                else {
                    const char* digits = t->name + plen; bool found = false;
                    for (long i = 0; i < (long)std::strlen(digits); ++i)
                        if ((char)(digits[i] - '0') == level) { found = true; break; }
                    t->limbo = !found;
                }
            }
        }
    }
};
int main()
{
    Thing a = { "Dress12", false, -1 }, b = { "dress3", false, -1 }, c = { "Chair", false, -1 }, d = { "Dress1", true, -1 };
    House h = { 0, { &a, &b, &c, &d }, 4 };
    h.SetupDressingAtLevel(2);
    if (h.DressLevel != 2 || a.limbo != 0 || b.limbo != 1 || c.limbo != -1 || d.limbo != -1) { std::printf("006c0330_TEST FAIL\n"); return 1; }
    h.SetupDressingAtLevel(-1);
    if (a.limbo != 0 || b.limbo != 0) { std::printf("006c0330_TEST FAIL\n"); return 1; }
    std::printf("006c0330_TEST PASS\n");
    return 0;
}