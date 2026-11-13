// Behaviour test for 0x005841F9 CTCInventory::GetItemDescription(CWideString& text, long itemDefIndex, bool hidePrice) const.
// A plain C++ model of the row's logic (text as an `unsigned short` buffer; the text bank returns the symbol itself; the
// details text is "<details>"), checked against hand-built expected strings for: price shown / hidden, the shop buy price
// overriding the default price only when the hero stands in a live shop that stocks the item, a hair card with worn
// modifiers on the same and on another body area, a hair card whose object has no appearance modifier, a tattoo card (no
// change shown), and an ordinary item (no appearance lines).
#include <cstdio>

struct WText {
    unsigned short Buffer[1024];
    long Length;
    WText() : Length(0) { Buffer[0] = 0; }
    void Append(const char* s) { while (*s) Buffer[Length++] = (unsigned short)(unsigned char)*s++; Buffer[Length] = 0; }
    void AppendLong(long v)
    {
        char tmp[32]; std::sprintf(tmp, "%ld", v); Append(tmp);
    }
    bool Equals(const char* s) const
    {
        long i = 0;
        for (; s[i]; ++i) if (i >= Length || Buffer[i] != (unsigned short)(unsigned char)s[i]) return false;
        return i == Length;
    }
};

struct ModifierDef { bool Present; long CoversBodyAreaFlags; long AppearanceType; float Attractiveness, Scariness, Goodstrength; };
struct ItemModel {
    bool HasStockDef; long DefaultPrice;
    bool InVolume, ShopThingAlive, ShopHasShopTC, ShopStocksItem; long ShopBuyPrice;
    bool HasHairCard; bool HairObjectDefExists; ModifierDef HairModifier;
    bool HasAppearanceTC; ModifierDef Worn[4]; long WornCount;
    bool HasTattooCard; float TattooAttractiveness, TattooScariness, TattooGoodstrength;
};

static void AppendStat(WText& text, const char* label, float item, float cur, bool showChange)
{
    long value = (long)(item * 100.0f);
    long change = value - (long)(cur * 100.0f);
    text.Append(label);
    text.Append(" ");
    if (value > 0) text.Append("+");
    text.AppendLong(value);
    if (showChange) {
        text.Append(" (");
        if (change > 0) text.Append("+");
        text.AppendLong(change);
        text.Append(")");
    }
    text.Append("\n");
}

// mirrors the row's control flow statement by statement
static void GetItemDescription(const ItemModel& m, WText& text, bool hidePrice)
{
    if (!hidePrice) {
        long price = 0;
        if (m.HasStockDef) price = m.DefaultPrice;
        if (m.InVolume) {
            if (m.ShopThingAlive && m.ShopHasShopTC && m.ShopStocksItem)
                price = m.ShopBuyPrice;
        }
        text.Append("TEXT_GUI_PRICE");
        text.Append(": ");
        text.AppendLong(price);
        text.Append("\n");
    }
    if (m.HasHairCard || m.HasTattooCard) {
        float itemA = 0.0f, curA = 0.0f, itemS = 0.0f, curS = 0.0f, itemG = 0.0f, curG = 0.0f;
        bool showChange = false;
        if (m.HasHairCard) {
            if (m.HairObjectDefExists && m.HairModifier.Present) {
                itemA = m.HairModifier.Attractiveness; itemS = m.HairModifier.Scariness; itemG = m.HairModifier.Goodstrength;
                if (m.HasAppearanceTC) {
                    for (long i = 0; i < m.WornCount; ++i) {
                        const ModifierDef& w = m.Worn[i];
                        if (w.Present && w.AppearanceType == m.HairModifier.AppearanceType &&
                            w.CoversBodyAreaFlags == m.HairModifier.CoversBodyAreaFlags) {
                            curA += w.Attractiveness; curS += w.Scariness; curG += w.Goodstrength;
                        }
                    }
                }
                showChange = true;
            }
        } else {
            itemA = m.TattooAttractiveness; itemS = m.TattooScariness; itemG = m.TattooGoodstrength;
            showChange = false;
        }
        AppendStat(text, "TEXT_GUI_MENU_ATTRACTIVENESS", itemA, curA, showChange);
        AppendStat(text, "TEXT_GUI_MENU_SCARINESS", itemS, curS, showChange);
        AppendStat(text, "TEXT_GUI_MENU_ALIGNMENT_MODIFIER", itemG, curG, showChange);
    }
    text.Append("<details>");
}

static ItemModel Plain()
{
    ItemModel m = {};
    return m;
}

int main()
{
    int fails = 0;
    // 1. ordinary item, default price, not in a shop
    {
        ItemModel m = Plain(); m.HasStockDef = true; m.DefaultPrice = 120;
        WText t; GetItemDescription(m, t, false);
        if (!t.Equals("TEXT_GUI_PRICE: 120\n<details>")) ++fails;
        WText h; GetItemDescription(m, h, true);
        if (!h.Equals("<details>")) ++fails;
    }
    // 2. shop price only when the shop is alive, has the shop component and stocks the item
    {
        ItemModel m = Plain(); m.HasStockDef = true; m.DefaultPrice = 120; m.InVolume = true; m.ShopBuyPrice = 95;
        m.ShopThingAlive = true; m.ShopHasShopTC = true; m.ShopStocksItem = true;
        WText t; GetItemDescription(m, t, false);
        if (!t.Equals("TEXT_GUI_PRICE: 95\n<details>")) ++fails;
        m.ShopStocksItem = false;
        WText u; GetItemDescription(m, u, false);
        if (!u.Equals("TEXT_GUI_PRICE: 120\n<details>")) ++fails;
        m.ShopStocksItem = true; m.ShopThingAlive = false;
        WText v; GetItemDescription(m, v, false);
        if (!v.Equals("TEXT_GUI_PRICE: 120\n<details>")) ++fails;
        m.HasStockDef = false;
        WText w; GetItemDescription(m, w, false);
        if (!w.Equals("TEXT_GUI_PRICE: 0\n<details>")) ++fails;
    }
    // 3. hair card against the worn modifiers of the same body area (one other area ignored), price hidden
    {
        ItemModel m = Plain(); m.HasHairCard = true; m.HairObjectDefExists = true;
        ModifierDef hair = { true, 4, 0, 0.5f, -0.25f, 0.0f }; m.HairModifier = hair;
        m.HasAppearanceTC = true; m.WornCount = 2;
        ModifierDef same = { true, 4, 0, 0.75f, 0.25f, -0.5f }; ModifierDef other = { true, 8, 0, 3.0f, 3.0f, 3.0f };
        m.Worn[0] = same; m.Worn[1] = other;
        WText t; GetItemDescription(m, t, true);
        if (!t.Equals("TEXT_GUI_MENU_ATTRACTIVENESS +50 (-25)\nTEXT_GUI_MENU_SCARINESS -25 (-50)\n"
                      "TEXT_GUI_MENU_ALIGNMENT_MODIFIER 0 (+50)\n<details>")) ++fails;
    }
    // 4. hair card whose hair object has no appearance modifier: zero lines, no change shown
    {
        ItemModel m = Plain(); m.HasHairCard = true; m.HairObjectDefExists = true; m.HasTattooCard = true;
        m.TattooAttractiveness = 9.0f;
        WText t; GetItemDescription(m, t, true);
        if (!t.Equals("TEXT_GUI_MENU_ATTRACTIVENESS 0\nTEXT_GUI_MENU_SCARINESS 0\nTEXT_GUI_MENU_ALIGNMENT_MODIFIER 0\n<details>")) ++fails;
    }
    // 5. tattoo card: its own values, no change
    {
        ItemModel m = Plain(); m.HasTattooCard = true; m.TattooAttractiveness = 1.5f; m.TattooScariness = -0.5f;
        m.TattooGoodstrength = 0.25f; m.HasStockDef = true; m.DefaultPrice = 7;
        WText t; GetItemDescription(m, t, false);
        if (!t.Equals("TEXT_GUI_PRICE: 7\nTEXT_GUI_MENU_ATTRACTIVENESS +150\nTEXT_GUI_MENU_SCARINESS -50\n"
                      "TEXT_GUI_MENU_ALIGNMENT_MODIFIER +25\n<details>")) ++fails;
    }
    if (fails) { std::printf("005841f9_TEST FAIL\n"); return 1; }
    std::printf("005841f9_TEST PASS\n");
    return 0;
}