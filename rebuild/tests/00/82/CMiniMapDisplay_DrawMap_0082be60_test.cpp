#include <cstdio>
#include <cstring>
// Behaviour test for CMiniMapDisplay::DrawMap (0x0082BE60): early-outs (no hero, hero flag, no graphic), the layer
// choice, the prison-courtyard switch on height 51 and the Headsman's Hill switch on y 4740, then sprite + markers.
struct C3DVector { float X, Y, Z; };
struct CHero { unsigned char Flags91; C3DVector Pos; const C3DVector& GetPosition() const { return Pos; } };
static CHero* g_hero = 0;
static long g_lastLayer = -1, g_lastGraphic = -1, g_sprites = 0, g_markers = 0;
static long Find(const char* name)
{
    if (std::strcmp(name, "MINIMAP_PRISONCOURTYARD2") == 0) return 22;
    if (std::strcmp(name, "MINIMAP_PRISONCOURTYARD1") == 0) return 11;
    return -1;
}
struct CMiniMap {
    long MiniMapGraphic;
    bool C1, C2, North, South;
    void DrawMap(bool front)
    {
        CHero* hero = g_hero;
        if (!hero || (hero->Flags91 & 1) || MiniMapGraphic <= 0) return;
        long layer = 0x44;
        if (!front) layer = 0x47;
        if (C1 && hero->GetPosition().Z > 51.0f) { MiniMapGraphic = Find("MINIMAP_PRISONCOURTYARD2"); C1 = false; C2 = true; }
        else if (C2 && hero->GetPosition().Z <= 51.0f) { MiniMapGraphic = Find("MINIMAP_PRISONCOURTYARD1"); C1 = true; C2 = false; }
        else if (North && hero->GetPosition().Y <= 4740.0f) { North = false; South = true; }
        else if (South && hero->GetPosition().Y > 4740.0f) { North = true; South = false; }
        g_lastLayer = layer; g_lastGraphic = MiniMapGraphic; g_sprites++;
        g_markers++;
    }
};
static int Fail() { std::printf("0082be60_TEST FAIL\n"); return 1; }
int main()
{
    CMiniMap m = { 5, true, false, false, false };
    m.DrawMap(true);
    if (g_sprites != 0) return Fail();                       // no hero
    CHero hero = { 0, { 0.0f, 5000.0f, 60.0f } };
    g_hero = &hero;
    hero.Flags91 = 1; m.DrawMap(true);
    if (g_sprites != 0) return Fail();                       // flagged hero
    hero.Flags91 = 2; m.DrawMap(true);
    if (g_sprites != 1 || g_lastLayer != 0x44 || g_lastGraphic != 22 || m.C1 || !m.C2) return Fail();
    hero.Pos.Z = 51.0f; m.DrawMap(false);
    if (g_lastLayer != 0x47 || g_lastGraphic != 11 || !m.C1 || m.C2) return Fail();
    m.C1 = false; m.North = true; m.DrawMap(true);           // y 5000 > 4740: north stays
    if (!m.North || m.South) return Fail();
    hero.Pos.Y = 4740.0f; m.DrawMap(true);
    if (m.North || !m.South) return Fail();
    hero.Pos.Y = 4741.0f; m.DrawMap(true);
    if (!m.North || m.South) return Fail();
    m.MiniMapGraphic = 0; m.DrawMap(true);
    if (g_sprites != 5 || g_markers != 5) return Fail();
    std::printf("0082be60_TEST PASS\n");
    return 0;
}