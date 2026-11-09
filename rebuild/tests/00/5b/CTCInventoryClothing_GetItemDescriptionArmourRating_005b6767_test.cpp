// Behaviour test for 0x005B6767 CTCInventoryClothing::GetItemDescriptionArmourRating (C++03): appends the text bank's
// TEXT_GUI_MENU_ARMOUR_RATING string, " ", the item's armour value, " (", a "+" only when the item beats what is worn
// (difference > 0), the signed difference and ")" to the caller's text.
#include <cstdio>
#include <string>

struct ArgPasser { long total_item_armor_value_final; long total_cur_armor_value_final; };

static std::string GetTextBySymbol(const std::string& symbol)
{
    return symbol == "TEXT_GUI_MENU_ARMOUR_RATING" ? std::string("Armour") : std::string("?");
}
static std::string IntToString(long value)
{
    char buffer[32];
    std::sprintf(buffer, "%ld", value);
    return buffer;
}
static void GetItemDescriptionArmourRating(std::string& text, const ArgPasser& values, const bool* game)
{
    long difference = values.total_item_armor_value_final - values.total_cur_armor_value_final;
    if (game == 0)
        return;
    text += GetTextBySymbol("TEXT_GUI_MENU_ARMOUR_RATING");
    text += " ";
    text += IntToString(values.total_item_armor_value_final);
    text += " (";
    if (difference > 0)
        text += "+";
    text += IntToString(difference);
    text += ")";
}

static int Check(long item, long cur, const char* prefix, const char* expected)
{
    static const bool game = true;
    ArgPasser values = { item, cur };
    std::string text = prefix;
    GetItemDescriptionArmourRating(text, values, &game);
    if (text != expected) { std::printf("%ld/%ld: got '%s' want '%s'\n", item, cur, text.c_str(), expected); return 1; }
    return 0;
}

int main()
{
    int fails = 0;
    fails += Check(12, 5, "", "Armour 12 (+7)");
    fails += Check(5, 12, "Hat\n", "Hat\nArmour 5 (-7)");
    fails += Check(8, 8, "", "Armour 8 (0)");
    fails += Check(0, 3, "", "Armour 0 (-3)");
    fails += Check(1, 0, "", "Armour 1 (+1)");
    if (fails) { std::printf("005b6767_TEST FAIL %d\n", fails); return 1; }
    std::printf("005b6767_TEST PASS\n");
    return 0;
}