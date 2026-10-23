// Behaviour test for CTCInGameMenu::ConstructSaveGameFileDescription (0x0047EEE4): a verified save (main file, and the "."
// companion only when it exists) gets "<profile label>:<name>\n<date><sep><time>\n<region label>:<region>\n<play time>",
// the region text coming from the text bank (through the group's first entry when the entry is a group) or, when the
// bank has no entry, from the raw region name; an unverified save gets the invalid-save text with the file name substituted.
#include <cstdio>
#include <string>

static bool g_mainValid, g_companionExists, g_companionValid;
static unsigned long g_entry;
static bool g_isGroup;
static int g_verifyCalls;

static bool Verify(bool companion)
{
    ++g_verifyCalls;
    return companion ? g_companionValid : g_mainValid;
}
static std::string BankText(unsigned long id)
{
    char buf[32];
    std::sprintf(buf, "R%lu", id);
    return buf;
}
static std::string Model(const std::string& file, const std::string& region)
{
    std::string d;
    bool hasCompanion = false;
    if (g_companionExists)
        hasCompanion = true;
    bool valid = Verify(false) && (!hasCompanion || Verify(true));
    if (valid) {
        d = std::string("Name") + ":";
        d += "Hero";
        d += "\n";
        std::string when("1/2/3");
        when += " ";
        when += std::string("4:05");
        d += when;
        d += "\n";
        d += std::string("Region") + ":";
        std::string r;
        unsigned long entry = g_entry;
        if (entry > 0) {
            if (g_isGroup)
                r = BankText(entry * 10);
            else
                r = BankText(entry);
        } else
            r = region;
        d += r;
        d += "\n";
        d += "1h";
    } else {
        d = "Invalid ";
        d += file;
    }
    return d;
}

int main()
{
    int fails = 0;
    g_mainValid = true; g_companionExists = false; g_companionValid = false; g_entry = 0;
    if (Model("f", "Oakvale") != "Name:Hero\n1/2/3 4:05\nRegion:Oakvale\n1h") ++fails;
    if (g_verifyCalls != 1) ++fails;
    g_verifyCalls = 0; g_companionExists = true;
    if (Model("f", "x") != "Invalid f") ++fails;
    if (g_verifyCalls != 2) ++fails;
    g_companionValid = true; g_entry = 7; g_isGroup = false;
    if (Model("f", "x") != "Name:Hero\n1/2/3 4:05\nRegion:R7\n1h") ++fails;
    g_isGroup = true;
    if (Model("f", "x") != "Name:Hero\n1/2/3 4:05\nRegion:R70\n1h") ++fails;
    g_mainValid = false; g_verifyCalls = 0;
    if (Model("save3", "x") != "Invalid save3") ++fails;
    if (g_verifyCalls != 1) ++fails;
    if (fails == 0)
        std::printf("0047eee4_TEST PASS\n");
    else
        std::printf("0047eee4_TEST FAIL %d\n", fails);
    return fails;
}