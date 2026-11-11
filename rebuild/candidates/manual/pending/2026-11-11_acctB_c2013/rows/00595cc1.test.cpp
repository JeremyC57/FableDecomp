// Behaviour test for 0x00595CC1 NUISystem::CFrontEndManager::ConstructFileDescription(unsigned long, CComponent*, CComponent*,
// CWideString&): a self-contained host model of the row's logic (wide text as unsigned short; plain C++98 for g++ and VC7.1).
#include <cstdio>
#include <cstring>

struct WStr {                                   // CWideString stand-in
    unsigned short t[256]; int n;
    WStr() : n(0) { t[0] = 0; }
    WStr(const char* s) : n(0) { t[0] = 0; *this += s; }
    WStr& operator+=(const char* s) { while (*s && n < 255) t[n++] = (unsigned short)(unsigned char)*s++; t[n] = 0; return *this; }
    WStr& operator+=(const WStr& o) { for (int i = 0; i < o.n && n < 255; ++i) t[n++] = o.t[i]; t[n] = 0; return *this; }
    bool operator==(const WStr& o) const { if (n != o.n) return false; for (int i = 0; i < n; ++i) if (t[i] != o.t[i]) return false; return true; }
    bool operator!=(const WStr& o) const { return !(*this == o); }
    bool Equals(const char* s) const { WStr o(s); return *this == o; }
};
static WStr operator+(const WStr& a, const char* b) { WStr r = a; r += b; return r; }

struct Component { int kind; long graphic; Component* children[4]; int nchildren; Component* fake[4]; int nfake; WStr value;
    Component(int k) : kind(k), graphic(-1), nchildren(0), nfake(0) {} };
enum { BOX = 1, TEXT, DRAW_PIC, SAVE_PIC, TEXT_VIEWPORT, SCREENSHOT_VIEWPORT };

struct Model {
    // inputs
    WStr profile_name, empty_slot; long play_time;
    const char* region_symbol;     // FileRegionNames[index]
    long region_entry;             // FindIndexBySymbolCRC result (0 = unknown)
    bool region_is_group;          // GetEntryDataType
    const char* minimap_name;      // FileRegionMiniMapNames[index]
    long minimap_graphic;          // MiniMapGraphics lookup (-2 = not in the map)
    bool save_ok, header_exists, header_ok;
    Component* text_viewport; Component* screenshot_viewport;
    Component* made[8]; int nmade;
    Model() : nmade(0) {}
    Component* Create(int kind) { made[nmade] = new Component(kind); return made[nmade++]; }
    WStr TextForEntry(long id) { WStr s("Region#"); char b[8]; std::sprintf(b, "%ld", id); s += b; return s; }
    void Construct(Component* button, const WStr& name) {
        Component* box = Create(BOX);
        Component* text = Create(TEXT);
        WStr description = WStr("Profile") + " : ";
        description += profile_name;
        description += "\n";
        WStr when("1/2/2004"); when += " "; when += WStr("12:00");
        description += when;
        description += "\n";
        description += WStr("Region") + " : ";
        WStr region;
        if (region_entry > 0) {
            if (!region_is_group) region = TextForEntry(region_entry);
            else region = TextForEntry(region_entry * 10);          // the group's first text id
        } else region = WStr(region_symbol);
        description += region;
        description += "\n";
        char b[16]; std::sprintf(b, "%ldh", play_time); description += b;
        text->value = description;
        if (text_viewport) text_viewport->fake[text_viewport->nfake++] = text;
        bool hdr = false;
        if (header_exists) hdr = true;
        bool valid = save_ok && (!hdr || header_ok);
        Component* picture = 0;
        if (valid) {
            if (name != empty_slot) {
                picture = Create(DRAW_PIC);
                if (std::strcmp(minimap_name, "") != 0) {
                    if (minimap_graphic != -2 && (unsigned long)minimap_graphic > 0) picture->graphic = minimap_graphic;
                }
            }
        }
        if (!picture) picture = Create(SAVE_PIC);
        if (screenshot_viewport) screenshot_viewport->fake[screenshot_viewport->nfake++] = picture;
        button->children[button->nchildren++] = picture;
        button->children[button->nchildren++] = box;
        if (name != empty_slot) button->children[button->nchildren++] = text;
    }
    ~Model() { for (int i = 0; i < nmade; ++i) delete made[i]; }
};

static int fails = 0;
static void check(bool c, int line) { if (!c) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(c) check((c), __LINE__)

int main()
{
    {   // a valid save with a group region and a known minimap
        Model m; Component tv(TEXT_VIEWPORT), sv(SCREENSHOT_VIEWPORT), button(BOX);
        m.profile_name = WStr("Hero"); m.empty_slot = WStr("Empty"); m.play_time = 3;
        m.region_symbol = "OAKVALE"; m.region_entry = 4; m.region_is_group = true; m.minimap_name = "MAP_OAK"; m.minimap_graphic = 77;
        m.save_ok = true; m.header_exists = true; m.header_ok = true; m.text_viewport = &tv; m.screenshot_viewport = &sv;
        m.Construct(&button, WStr("Save1"));
        CHECK(m.made[1]->value.Equals("Profile : Hero\n1/2/2004 12:00\nRegion : Region#40\n3h"));
        CHECK(tv.nfake == 1 && tv.fake[0] == m.made[1]);
        CHECK(sv.nfake == 1 && sv.fake[0]->kind == DRAW_PIC && sv.fake[0]->graphic == 77);
        CHECK(button.nchildren == 3 && button.children[0]->kind == DRAW_PIC && button.children[1]->kind == BOX && button.children[2]->kind == TEXT);
    }
    {   // the header exists but fails: default picture; unknown region falls back to its symbol
        Model m; Component tv(TEXT_VIEWPORT), button(BOX);
        m.empty_slot = WStr("Empty"); m.play_time = 0; m.region_symbol = "X"; m.region_entry = 0; m.region_is_group = false;
        m.minimap_name = "M"; m.minimap_graphic = 5; m.save_ok = true; m.header_exists = true; m.header_ok = false;
        m.text_viewport = &tv; m.screenshot_viewport = 0;
        m.Construct(&button, WStr("Save2"));
        CHECK(m.made[1]->value.Equals("Profile : \n1/2/2004 12:00\nRegion : X\n0h"));
        CHECK(button.nchildren == 3 && button.children[0]->kind == SAVE_PIC);
    }
    {   // the empty slot: default picture, no text child; a text region entry; minimap not in the map
        Model m; Component button(BOX);
        m.empty_slot = WStr("Empty"); m.play_time = 1; m.region_symbol = "Y"; m.region_entry = 9; m.region_is_group = false;
        m.minimap_name = "M"; m.minimap_graphic = -2; m.save_ok = true; m.header_exists = false; m.header_ok = false;
        m.text_viewport = 0; m.screenshot_viewport = 0;
        m.Construct(&button, WStr("Empty"));
        CHECK(m.made[1]->value.Equals("Profile : \n1/2/2004 12:00\nRegion : Region#9\n1h"));
        CHECK(button.nchildren == 2 && button.children[0]->kind == SAVE_PIC && button.children[1]->kind == BOX);
    }
    {   // valid save, minimap graphic 0 is not applied
        Model m; Component button(BOX);
        m.empty_slot = WStr("Empty"); m.play_time = 2; m.region_symbol = "Z"; m.region_entry = 0; m.region_is_group = false;
        m.minimap_name = "M"; m.minimap_graphic = 0; m.save_ok = true; m.header_exists = false; m.header_ok = false;
        m.text_viewport = 0; m.screenshot_viewport = 0;
        m.Construct(&button, WStr("Save3"));
        CHECK(button.children[0]->kind == DRAW_PIC && button.children[0]->graphic == -1 && button.nchildren == 3);
    }
    if (fails) { std::printf("00595cc1_TEST FAIL\n"); return 1; }
    std::printf("00595cc1_TEST PASS\n");
    return 0;
}
