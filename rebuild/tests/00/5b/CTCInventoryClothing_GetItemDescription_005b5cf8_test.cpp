// Behaviour test for 0x005B5CF8 CTCInventoryClothing::GetItemDescription: builds the stat block text from the ArgPasser
// values (armour rating, suit parts, price unless hidden, attractiveness / scariness / alignment with their signed changes) plus the description.
#include <cstdio>
#include <cstring>

struct Text {
    char buf[512];
    Text() { buf[0] = 0; }
    Text& operator+=(const char* s) { std::strcat(buf, s); return *this; }
};
static const char* Lookup(const char* symbol)
{
    if (std::strcmp(symbol, "TEXT_GUI_MENU_PARTS_ACQUIRED") == 0) return "Parts:";
    if (std::strcmp(symbol, "TEXT_GUI_MENU_ARMOUR_RATING") == 0) return "Armour";
    if (std::strcmp(symbol, "TEXT_GUI_MENU_PRICE") == 0) return "Price";
    if (std::strcmp(symbol, "TEXT_GUI_MENU_ATTRACTIVENESS") == 0) return "Attr";
    if (std::strcmp(symbol, "TEXT_GUI_MENU_SCARINESS") == 0) return "Scar";
    return "Align";
}
static const char* IntText(long v) { static char b[8][16]; static int n; char* p = b[n++ & 7]; std::sprintf(p, "%ld", v); return p; }
struct ArgPasser {
    bool is_suit; long num_suit_parts, max_suit_parts, total_item_armor_value_final, total_cur_armor_value_final;
    float total_item_attractiveness_value_final, total_cur_attractiveness_value_final;
    float total_item_scariness_value_final, total_cur_scariness_value_final;
    float total_item_goodstrength_value_final, total_cur_goodstrength_value_final;
    long total_price;
};
struct Clothing {
    ArgPasser values;
    void GetItemDescription(Text& out, long defIndex, bool hidePrice)
    {
        ArgPasser v = values;
        long change = v.total_item_armor_value_final - v.total_cur_armor_value_final;
        out += Lookup("TEXT_GUI_MENU_ARMOUR_RATING"); out += " "; out += IntText(v.total_item_armor_value_final); out += " (";
        if (change > 0) out += "+";
        out += IntText(change); out += ")\n";
        if (v.is_suit) {
            out += Lookup("TEXT_GUI_MENU_PARTS_ACQUIRED"); out += " ";
            out += IntText(v.num_suit_parts); out += "/"; out += IntText(v.max_suit_parts); out += "\n";
        }
        if (!hidePrice) {
            out += Lookup("TEXT_GUI_MENU_PRICE");
            Text t; t += " "; t += IntText(v.total_price); out += t.buf;
            out += "\n";
        }
        change = (long)(v.total_item_attractiveness_value_final - v.total_cur_attractiveness_value_final);
        out += Lookup("TEXT_GUI_MENU_ATTRACTIVENESS"); out += " ";
        if (v.total_item_attractiveness_value_final > 0.0f) out += "+";
        out += IntText((long)v.total_item_attractiveness_value_final); out += " (";
        if (change > 0) out += "+";
        out += IntText(change); out += ")\n";
        change = (long)(v.total_item_scariness_value_final - v.total_cur_scariness_value_final);
        out += Lookup("TEXT_GUI_MENU_SCARINESS"); out += " ";
        if (v.total_item_scariness_value_final > 0.0f) out += "+";
        out += IntText((long)v.total_item_scariness_value_final); out += " (";
        if (change > 0) out += "+";
        out += IntText(change); out += ")\n";
        change = (long)(v.total_item_goodstrength_value_final - v.total_cur_goodstrength_value_final);
        out += Lookup("TEXT_GUI_MENU_ALIGNMENT_MODIFIER"); out += " ";
        if (v.total_item_goodstrength_value_final > 0.0f) out += "+";
        out += IntText((long)v.total_item_goodstrength_value_final); out += " (";
        if (change > 0) out += "+";
        out += IntText(change); out += ")\n";
        out += defIndex == 5 ? "Hat" : "?";
    }
};

int main()
{
    Clothing c; std::memset(&c, 0, sizeof(c));
    c.values.total_item_armor_value_final = 10; c.values.total_cur_armor_value_final = 4; c.values.total_price = 99;
    c.values.is_suit = true; c.values.num_suit_parts = 2; c.values.max_suit_parts = 4;
    c.values.total_item_attractiveness_value_final = 3.0f; c.values.total_cur_attractiveness_value_final = 1.0f;
    c.values.total_item_scariness_value_final = -2.0f; c.values.total_cur_scariness_value_final = 0.0f;
    c.values.total_item_goodstrength_value_final = 0.0f; c.values.total_cur_goodstrength_value_final = 1.0f;
    Text out;
    c.GetItemDescription(out, 5, false);
    Text hidden;
    c.GetItemDescription(hidden, 5, true);
    const char* wantHidden = "Armour 10 (+6)\nParts: 2/4\nAttr +3 (+2)\nScar -2 (-2)\nAlign 0 (-1)\nHat";
    if (std::strcmp(hidden.buf, wantHidden) != 0) { std::printf("005b5cf8_TEST FAIL\n"); return 1; }
    const char* want = "Armour 10 (+6)\nParts: 2/4\nPrice 99\nAttr +3 (+2)\nScar -2 (-2)\nAlign 0 (-1)\nHat";
    if (std::strcmp(out.buf, want) != 0) { std::printf("005b5cf8_TEST FAIL\n"); return 1; }
    std::printf("005b5cf8_TEST PASS\n");
    return 0;
}