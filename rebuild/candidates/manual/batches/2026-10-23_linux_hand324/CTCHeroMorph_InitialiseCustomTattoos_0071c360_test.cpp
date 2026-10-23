// Behaviour test for CTCHeroMorph::InitialiseCustomTattoos (0x0071C360): each of the five custom tattoo cards found by name
// copies its tattoo sub-def's file; then the tutorial text is copied from the custom-tattoo dir to the save dir only when the
// destination is missing, the source exists and the disk has room. Every def reference is released (counts return to 0).
#include <cstdio>
#include <cstring>

struct Def {
    const char* name;
    long refs;
    int copies;
    Def* sub;
};
static Def g_tattoos[5];
static Def g_cards[5];
static const char* g_names[5] = {
    "OBJECT_TATTOO_CARD_FACE_CUSTOM_01", "OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", "OBJECT_TATTOO_CARD_BACK_CUSTOM_01",
    "OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", "OBJECT_TATTOO_CARD_LEGS_CUSTOM_01"
};
static bool g_present[5];
static bool g_destExists, g_srcExists, g_room;
static int g_fileCopies;
static char g_order[128];

struct Ptr {
    Def* p;
    Ptr() : p(0) { }
    explicit Ptr(Def* d) : p(d) { if (p) ++p->refs; }
    ~Ptr() { if (p) --p->refs; }
};
static Def* Lookup(const char* name)
{
    for (int i = 0; i < 5; ++i)
        if (g_present[i] && std::strcmp(g_names[i], name) == 0)
            return &g_cards[i];
    return 0;
}
static void GetSubDef(Def* card, Ptr* out)
{
    if (card->sub) {
        ++card->sub->refs;
        out->p = card->sub;
    }
}
static void Model()
{
    for (int i = 0; i < 5; ++i) {
        Ptr card(Lookup(g_names[i]));
        if (card.p) {
            Ptr tattoo;
            GetSubDef(card.p, &tattoo);
            ++tattoo.p->copies;
            std::strcat(g_order, g_names[i] + 19);
        }
    }
    if (!g_destExists && g_srcExists && g_room)
        ++g_fileCopies;
}

static void Reset()
{
    for (int i = 0; i < 5; ++i) {
        g_tattoos[i].refs = 0; g_tattoos[i].copies = 0;
        g_cards[i].refs = 0; g_cards[i].copies = 0; g_cards[i].sub = &g_tattoos[i];
        g_present[i] = true;
    }
    g_fileCopies = 0;
    g_order[0] = 0;
}

int main()
{
    int fails = 0;
    Reset();
    g_destExists = false; g_srcExists = true; g_room = true;
    Model();
    for (int i = 0; i < 5; ++i) {
        if (g_tattoos[i].copies != 1 || g_tattoos[i].refs != 0 || g_cards[i].refs != 0) ++fails;
    }
    if (std::strcmp(g_order, "FACE_CUSTOM_01CHEST_CUSTOM_01BACK_CUSTOM_01ARMS_CUSTOM_01LEGS_CUSTOM_01") != 0) ++fails;
    if (g_fileCopies != 1) ++fails;

    Reset();
    g_present[1] = false; g_present[3] = false;
    g_destExists = true;
    Model();
    if (g_tattoos[1].copies != 0 || g_tattoos[3].copies != 0 || g_tattoos[0].copies != 1 || g_tattoos[4].copies != 1) ++fails;
    if (g_fileCopies != 0) ++fails;

    Reset();
    g_destExists = false; g_srcExists = false;
    Model();
    if (g_fileCopies != 0) ++fails;
    g_srcExists = true; g_room = false;
    Reset();
    Model();
    if (g_fileCopies != 0) ++fails;

    if (fails == 0)
        std::printf("0071c360_TEST PASS\n");
    else
        std::printf("0071c360_TEST FAIL %d\n", fails);
    return 0;
}
