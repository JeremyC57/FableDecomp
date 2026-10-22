// Behaviour test for 0x00594FDB: the about message is part 1's entries, the registry string (when not empty) and part 2's entries, one per line.
#include <cstdio>
#include <cstring>
struct CText {
    char s[512];
    CText() { s[0] = 0; }
    void Append(const char* t) { std::strcat(s, t); }
    long GetLength() const { return (long)std::strlen(s); }
};
struct CBank {
    const char* part1[4]; int count1; long tag1;
    const char* part2[4]; int count2; long tag2;
    long GetTag(const char* name) const { return std::strcmp(name, "TEXT_GUI_MENU_ABOUT_MESSAGE_PART_1") == 0 ? tag1 : tag2; }
    int Count(long tag) const { return tag == tag1 ? count1 : count2; }
    const char* Entry(long tag, unsigned int i) const { return tag == tag1 ? part1[i] : part2[i]; }
};
static const char* g_registryValue = "";
static CText Generate(const CBank& bank)
{
    CText message;
    long tag = bank.GetTag("TEXT_GUI_MENU_ABOUT_MESSAGE_PART_1");
    if (tag > 0) {
        for (unsigned int i = 0; i < (unsigned int)bank.Count(tag); ++i) {
            message.Append(bank.Entry(tag, i));
            message.Append("\n");
        }
    }
    CText value; value.Append(g_registryValue);
    if (value.GetLength()) {
        message.Append(value.s);
        message.Append("\n");
    }
    tag = bank.GetTag("TEXT_GUI_MENU_ABOUT_MESSAGE_PART_2");
    if (tag > 0) {
        for (unsigned int i = 0; i < (unsigned int)bank.Count(tag); ++i) {
            message.Append(bank.Entry(tag, i));
            message.Append("\n");
        }
    }
    return message;
}
int main()
{
    CBank bank;
    bank.part1[0] = "Fable"; bank.part1[1] = "Lionhead"; bank.count1 = 2; bank.tag1 = 5;
    bank.part2[0] = "Thanks"; bank.count2 = 1; bank.tag2 = 6;
    int bad = 0;
    g_registryValue = "v1.0";
    if (std::strcmp(Generate(bank).s, "Fable\nLionhead\nv1.0\nThanks\n") != 0) bad = 1;
    g_registryValue = "";
    if (std::strcmp(Generate(bank).s, "Fable\nLionhead\nThanks\n") != 0) bad = 1;
    bank.tag1 = 0;
    if (std::strcmp(Generate(bank).s, "Thanks\n") != 0) bad = 1;
    bank.tag2 = -1; bank.count1 = 0;
    if (std::strcmp(Generate(bank).s, "") != 0) bad = 1;
    if (bad) { std::printf("00594fdb_TEST FAIL\n"); return 1; }
    std::printf("00594fdb_TEST PASS\n");
    return 0;
}
