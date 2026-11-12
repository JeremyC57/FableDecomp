// Behaviour test for NPlayerGui::CDrawSpiritTargetingCursor::Draw (0x0064C869): the same loop over a small stand-alone model.
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Colour { unsigned char B, G, R, A; };
struct Thing { bool Dead; float Radius; Vec Pos; };
struct Targeting { Thing* Target; };
struct Hero { bool Dead; Targeting TC; };
struct Player { long Number; bool Main; Hero* PHero; };
struct Sprite { Vec Pos; long Graphic; unsigned short Flags; unsigned char Alpha, R, G, B; };
struct Engine { Sprite Sprites[8]; int Count; };
static bool Alive(const Hero* h) { return h != 0 && !h->Dead; }
static bool Alive(const Thing* t) { return t != 0 && !t->Dead; }
static Colour MultiplayerColour(long n)
{
    Colour c; c.A = 255;
    switch (n) {
    case 0: c.R = 255; c.G = 0; c.B = 0; return c;
    case 1: c.R = 0; c.G = 0; c.B = 255; return c;
    case 2: c.R = 255; c.G = 255; c.B = 0; return c;
    case 3: c.R = 0; c.G = 255; c.B = 0; return c;
    }
    c.R = c.G = c.B = 255; return c;
}
struct Drawer {
    Player* Players[4];
    bool LetterBox;
    bool Drawing;
    long Graphic;
    Thing* PTargetedThings[4];
    void Draw(Engine& engine, float interpolate)
    {
        for (long i = 0; i < 4; ++i) {
            Player& player = *Players[i];
            const Hero* hero = player.PHero;
            if (player.Main || !Alive(hero))
                continue;
            const Targeting* targeting = &hero->TC;
            if (targeting->Target != PTargetedThings[i])
                PTargetedThings[i] = targeting->Target;
            if (Alive(PTargetedThings[i]) && !LetterBox) {
                Drawing = true;
                Thing* target = PTargetedThings[i];
                Vec pos = target->Pos;
                pos.X += interpolate;
                pos.Z = pos.Z + target->Radius + 0.35f;
                Colour colour = MultiplayerColour(player.Number);
                Sprite& s = engine.Sprites[engine.Count++];
                s.Pos = pos; s.Graphic = Graphic; s.Flags = 0x81; s.Alpha = 0xff; s.R = colour.R; s.G = colour.G; s.B = colour.B;
            }
        }
    }
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    Thing things[3] = { { false, 1.0f, { 1.0f, 2.0f, 3.0f } }, { true, 2.0f, { 4.0f, 5.0f, 6.0f } }, { false, 0.5f, { 7.0f, 8.0f, 9.0f } } };
    Hero heroes[4] = { { false, { &things[0] } }, { false, { &things[1] } }, { true, { &things[2] } }, { false, { &things[2] } } };
    Player players[4] = { { 0, true, &heroes[0] }, { 1, false, &heroes[1] }, { 2, false, &heroes[2] }, { 3, false, &heroes[3] } };
    Drawer d;
    for (int i = 0; i < 4; ++i) { d.Players[i] = &players[i]; d.PTargetedThings[i] = 0; }
    d.LetterBox = false; d.Drawing = false; d.Graphic = 77;
    Engine e; e.Count = 0;
    bool ok = true;
    d.Draw(e, 0.25f);
    // slot 0: main player skipped (its target is not remembered); slot 1: target remembered but dead, no sprite;
    // slot 2: dead hero skipped; slot 3: one sprite in green, raised by radius + 0.35.
    ok = ok && d.PTargetedThings[0] == 0 && d.PTargetedThings[1] == &things[1] && d.PTargetedThings[2] == 0 && d.PTargetedThings[3] == &things[2];
    ok = ok && e.Count == 1 && d.Drawing;
    ok = ok && Near(e.Sprites[0].Pos.X, 7.25f) && Near(e.Sprites[0].Pos.Y, 8.0f) && Near(e.Sprites[0].Pos.Z, 9.0f + 0.5f + 0.35f);
    ok = ok && e.Sprites[0].Graphic == 77 && e.Sprites[0].Flags == 0x81 && e.Sprites[0].Alpha == 0xff;
    ok = ok && e.Sprites[0].R == 0 && e.Sprites[0].G == 255 && e.Sprites[0].B == 0;
    // letter-box mode: targets are still followed, nothing is drawn and Drawing is left alone.
    d.LetterBox = true; d.Drawing = false; e.Count = 0;
    heroes[1].TC.Target = &things[0];
    d.Draw(e, 0.0f);
    ok = ok && e.Count == 0 && !d.Drawing && d.PTargetedThings[1] == &things[0];
    // out of letter-box: slots 1 (blue) and 3 (green) draw, in slot order; the revived hero 2 (yellow) joins.
    d.LetterBox = false; heroes[2].Dead = false;
    d.Draw(e, 0.0f);
    ok = ok && e.Count == 3 && d.Drawing;
    ok = ok && e.Sprites[0].R == 0 && e.Sprites[0].G == 0 && e.Sprites[0].B == 255 && Near(e.Sprites[0].Pos.Z, 3.0f + 1.0f + 0.35f);
    ok = ok && e.Sprites[1].R == 255 && e.Sprites[1].G == 255 && e.Sprites[1].B == 0;
    ok = ok && e.Sprites[2].R == 0 && e.Sprites[2].G == 255 && e.Sprites[2].B == 0;
    if (!ok) { std::printf("0064c869_TEST FAIL\n"); return 1; }
    std::printf("0064c869_TEST PASS\n");
    return 0;
}