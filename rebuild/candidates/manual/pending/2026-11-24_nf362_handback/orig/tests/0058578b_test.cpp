// Behaviour test for 0x0058578B CTCInventory::OpenInventory(): a host model of the row's item-count decisions with stand-ins for
// the definition lookups (the count loop 0x0058680C..0x00586ACE and the entry tests that use its results): augmentation items
// are skipped, weapons are counted first, a hero suit counts once when any of its parts is owned, an appearance modifier counts
// as clothing for suit parts 1..5, expressions are ignored, then the card arms: no hair card and no tattoo card = a plain item,
// a hair card counts by its hair object's appearance flags (0x80 / 0x100 = hair, else 0x10 = facial hair, nothing when the hair
// def or its appearance sub-def is missing), a tattoo card counts as a tattoo; style count = hair + tattoo + facial hair; each
// menu entry shows its count only when its component exists and the count is positive.
#include <cstdio>

struct SItem {
    long Count;
    bool Augmentation, Weapon, HasSuit; long SuitPartsOwned[3]; long NumSuitParts;
    bool HasAppearance; long SuitPart; bool Expression;
    bool HairCard; bool HairDef; bool HairAppearance; long HairFlags; bool TattooCard;
};
struct SCounts { long weapons, items, clothing, facial_hair, hair, tattoo, style; bool show_weapons, show_items, show_clothing, show_style; };

static SCounts Count(const SItem* items, int n, bool weapons_tc, bool inventory_tc, bool clothing_tc)
{
    SCounts c = { 0, 0, 0, 0, 0, 0, 0, false, false, false, false };
    for (int i = 0; i < n; ++i) {
        const SItem& it = items[i];
        if (it.Augmentation)
            continue;
        if (it.Weapon) {
            c.weapons += it.Count;
        } else if (it.HasSuit) {
            for (long part = 0; part < it.NumSuitParts; ++part) {
                if (it.SuitPartsOwned[part] > 0) { c.clothing += it.Count; break; }
            }
        } else if (it.HasAppearance) {
            if (it.SuitPart > 0 && it.SuitPart <= 5)
                c.clothing += it.Count;
        } else if (!it.Expression) {
            if (!it.HairCard && !it.TattooCard) {
                c.items += it.Count;
            } else if (it.HairCard) {
                bool appearance = false;
                if (it.HairDef)
                    appearance = it.HairAppearance;
                if (appearance) {
                    long flags = it.HairFlags;
                    if ((flags & 0x80) || (flags & 0x100))
                        c.hair += it.Count;
                    else if (flags & 0x10)
                        c.facial_hair += it.Count;
                }
            } else {
                c.tattoo += it.Count;
            }
        }
    }
    c.show_weapons = weapons_tc && c.weapons > 0;
    c.show_items = inventory_tc && c.items > 0;
    c.show_clothing = clothing_tc && c.clothing > 0;
    c.style = c.hair + c.tattoo + c.facial_hair;
    c.show_style = inventory_tc && c.style > 0;
    return c;
}

static int g_fail = 0;
static void Check(bool c, int line) { if (!c) { std::printf("fail at line %d\n", line); ++g_fail; } }
#define CHECK(c) Check((c), __LINE__)
static SItem Item(long count) { SItem it; for (unsigned k = 0; k < sizeof(it); ++k) ((char*)&it)[k] = 0; it.Count = count; return it; }

int main()
{
    SItem items[14]; int n = 0;
    SItem aug = Item(100); aug.Augmentation = true; aug.Weapon = true; items[n++] = aug;           // skipped before the weapon test
    SItem sword = Item(2); sword.Weapon = true; sword.HasSuit = true; sword.NumSuitParts = 1; sword.SuitPartsOwned[0] = 1; items[n++] = sword;
    SItem suit = Item(3); suit.HasSuit = true; suit.NumSuitParts = 3; suit.SuitPartsOwned[1] = 1; suit.SuitPartsOwned[2] = 1; items[n++] = suit;   // counted once
    SItem suit0 = Item(5); suit0.HasSuit = true; suit0.NumSuitParts = 2; suit0.HasAppearance = true; suit0.SuitPart = 2; items[n++] = suit0;       // no part owned: nothing
    SItem hat = Item(7); hat.HasAppearance = true; hat.SuitPart = 5; items[n++] = hat;
    SItem wig = Item(11); wig.HasAppearance = true; wig.SuitPart = 6; items[n++] = wig;            // not a clothing part, and not a plain item either
    SItem zero = Item(13); zero.HasAppearance = true; zero.SuitPart = 0; items[n++] = zero;
    SItem expr = Item(17); expr.Expression = true; expr.TattooCard = true; items[n++] = expr;      // expressions are ignored
    SItem plain = Item(19); items[n++] = plain;
    SItem tattoo = Item(23); tattoo.TattooCard = true; items[n++] = tattoo;
    SItem hair = Item(29); hair.HairCard = true; hair.TattooCard = true; hair.HairDef = true; hair.HairAppearance = true; hair.HairFlags = 0x110; items[n++] = hair;
    SItem beard = Item(31); beard.HairCard = true; beard.HairDef = true; beard.HairAppearance = true; beard.HairFlags = 0x10; items[n++] = beard;
    SItem nodef = Item(37); nodef.HairCard = true; nodef.HairAppearance = true; nodef.HairFlags = 0x80; items[n++] = nodef;   // hair def missing
    SItem noflag = Item(41); noflag.HairCard = true; noflag.HairDef = true; noflag.HairAppearance = true; noflag.HairFlags = 0x20; items[n++] = noflag;

    SCounts c = Count(items, n, true, true, true);
    CHECK(c.weapons == 2);
    CHECK(c.clothing == 3 + 7);
    CHECK(c.items == 19);
    CHECK(c.tattoo == 23);
    CHECK(c.hair == 29);
    CHECK(c.facial_hair == 31);
    CHECK(c.style == 29 + 23 + 31);
    CHECK(c.show_weapons && c.show_items && c.show_clothing && c.show_style);

    hair.HairFlags = 0x80; items[10] = hair; beard.HairFlags = 0x90; items[11] = beard;
    c = Count(items, n, true, true, true);
    CHECK(c.hair == 29 + 31 && c.facial_hair == 0);                                            // the hair flags win over 0x10
    hair.HairAppearance = false; items[10] = hair;
    c = Count(items, n, true, true, true);
    CHECK(c.hair == 31 && c.tattoo == 23);                                                     // no appearance: nothing, and not a tattoo

    c = Count(items, n, false, false, false);
    CHECK(!c.show_weapons && !c.show_items && !c.show_clothing && !c.show_style && c.weapons == 2);
    c = Count(items, 1, true, true, true);
    CHECK(!c.show_weapons && !c.show_items && !c.show_clothing && !c.show_style && c.style == 0);

    if (g_fail == 0) std::printf("0058578b_TEST PASS\n");
    return g_fail;
}