// behaviour test for 00f0ca60: the 32-byte TeleportData's implicit copy constructor -- two strings
// copy-constructed, the seed and health copied, the carried-items vector deep-copied by its own
// copy constructor.
#include <cstdio>
#include <cstring>
static int g_stringCopies = 0;
static int g_vectorCopies = 0;
class CCharString { public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const CCharString& o) { ++g_stringCopies; std::strcpy(Text, o.Text); }
};
struct CCharStringVector {
    CCharString Items[4];
    int Count;
    CCharStringVector() : Count(0) { }
    CCharStringVector(const CCharStringVector& o) : Count(o.Count)
    {
        ++g_vectorCopies;
        int i = 0;
        for (i = 0; i < o.Count; ++i) std::strcpy(Items[i].Text, o.Items[i].Text);
    }
};
struct TeleportData {
    CCharString defName;
    unsigned long appearanceSeed;
    float health;
    CCharString scriptName;
    CCharStringVector itemsCarried;
};
int main()
{
    TeleportData a;
    std::strcpy(a.defName.Text, "CREATURE_TRADER");
    a.appearanceSeed = 7;
    a.health = 3.5f;
    std::strcpy(a.scriptName.Text, "Trader");
    a.itemsCarried.Count = 2;
    std::strcpy(a.itemsCarried.Items[0].Text, "Apple");
    std::strcpy(a.itemsCarried.Items[1].Text, "Sword");
    g_stringCopies = 0;
    TeleportData b(a);
    int ok = 1;
    if (g_stringCopies != 2 || g_vectorCopies != 1) ok = 0;
    if (std::strcmp(b.defName.Text, "CREATURE_TRADER") || std::strcmp(b.scriptName.Text, "Trader")) ok = 0;
    if (b.appearanceSeed != 7 || b.health != 3.5f || b.itemsCarried.Count != 2 || std::strcmp(b.itemsCarried.Items[1].Text, "Sword")) ok = 0;
    if (!ok) { std::printf("00f0ca60_TEST FAIL\n"); return 1; }
    std::printf("00f0ca60_TEST PASS\n");
    return 0;
}