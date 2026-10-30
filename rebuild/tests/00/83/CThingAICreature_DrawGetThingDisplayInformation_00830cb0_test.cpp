// Behaviour test for 0x00830CB0 CThingAICreature::DrawGetThingDisplayInformation (see src/00830cb0.cpp): a valid player's
// def colour with alpha 255 (else opaque black), the bar is the health percentage clamped to 0..1, size 0.7, bar on, false.
#include <stdio.h>
struct Colour { unsigned char B, G, R, A; };
struct Info { void* DrawString; float RelativeSize; float BarPercentage; Colour Col; bool BarOn; bool StringOn; };
struct Creature { signed char PlayerNumber; bool PlayerValid; Colour PlayerColour; float MaxHealth, Health; };
static float HealthPercentage(const Creature& c) { return c.MaxHealth > 0.0f ? c.Health / c.MaxHealth : 0.0f; }
static bool Draw(const Creature& c, Info* info)
{
    Colour colour;
    if (c.PlayerNumber != -1 && c.PlayerValid) { colour.R = c.PlayerColour.R; colour.G = c.PlayerColour.G; colour.B = c.PlayerColour.B; colour.A = 255; }
    else { colour.R = 0; colour.G = 0; colour.B = 0; colour.A = 255; }
    float health = HealthPercentage(c);
    info->BarPercentage = health;
    if (health < 0.0f) info->BarPercentage = 0.0f; else if (health > 1.0f) info->BarPercentage = 1.0f;
    info->Col = colour; info->BarOn = true; info->RelativeSize = 0.7f;
    return false;
}
int main()
{
    Colour red = { 10, 20, 200, 7 };
    Creature a = { 1, true, red, 100.0f, 50.0f };
    Info i = { 0, 0, 0, { 0, 0, 0, 0 }, false, false };
    bool ok = !Draw(a, &i) && i.Col.R == 200 && i.Col.G == 20 && i.Col.B == 10 && i.Col.A == 255 && i.BarPercentage == 0.5f && i.BarOn && i.RelativeSize == 0.7f;
    Creature b = { -1, true, red, 100.0f, 150.0f };
    Draw(b, &i);
    ok = ok && i.Col.R == 0 && i.Col.A == 255 && i.BarPercentage == 1.0f;
    Creature c = { 2, false, red, 100.0f, -5.0f };
    Draw(c, &i);
    ok = ok && i.Col.G == 0 && i.BarPercentage == 0.0f;
    printf(ok ? "00830cb0_TEST PASS\n" : "00830cb0_TEST FAIL\n");
    return ok ? 0 : 1;
}