// Behaviour test for NPlayerGui::CDrawAbilityButtonsPC::DrawExpressionButton (0x00658020): no hero, a dead hero or a hero without
// the expression component draws nothing; out-of-range indices draw nothing; otherwise the button is placed from the slot's type
// offsets and the row step, the expression def's graphic is used at full scale (default graphic at a third when the def is missing),
// and the layer is 0x45 for button type 1.
#include <cstdio>
#include <vector>

struct Vec { float x, y; };
struct Result { bool drawn; Vec pos; float scale; long button; long graphic; int layer; };

struct Setup {
    bool hasHero, dead, hasComponent;
    long columns, abilityButtons, defaultGraphic;
    Vec origin; float rowStep, rowOffset;
    Vec typeOffsets[4];
    long types[3];
    const long* defGraphics;    // graphic per global index, 0 = no def
};

Result Draw(const Setup& s, const std::vector<long>& expressions, long slot, long index, long row, float scale)
{
    Result r; r.drawn = false;
    if (!s.hasHero || s.dead || !s.hasComponent || index < 0)
        return r;
    long count = (long)expressions.size();
    if (index >= (count + 2) / s.columns * s.columns || (unsigned int)index >= expressions.size())
        return r;
    r.button = s.abilityButtons + index + 7;
    r.pos = s.origin;
    r.pos.x += s.typeOffsets[s.types[slot]].x;
    r.pos.y += s.typeOffsets[s.types[0]].y;
    r.pos.y += row * s.rowStep;
    r.pos.y += s.rowOffset;
    long def = s.defGraphics[expressions[index]];
    r.graphic = s.defaultGraphic; r.scale = scale;
    if (def) r.graphic = def; else r.scale = scale * 0.33333334f;
    r.layer = s.types[slot] == 1 ? 0x45 : 0x44;
    r.drawn = true;
    return r;
}

int main()
{
    int ok = 1;
    long graphics[4] = { 0, 21, 0, 23 };
    Setup s;
    s.hasHero = true; s.dead = false; s.hasComponent = true; s.columns = 3; s.abilityButtons = 5; s.defaultGraphic = 9;
    s.origin.x = 100.0f; s.origin.y = 200.0f; s.rowStep = 10.0f; s.rowOffset = 1.0f;
    for (int i = 0; i < 4; ++i) { s.typeOffsets[i].x = (float)(i * 2); s.typeOffsets[i].y = (float)(i * 3); }
    s.types[0] = 2; s.types[1] = 3; s.types[2] = 1;
    s.defGraphics = graphics;
    std::vector<long> exprs; exprs.push_back(1); exprs.push_back(2); exprs.push_back(3);
    Result r = Draw(s, exprs, 1, 0, 2, 1.0f);
    if (!r.drawn || r.button != 12 || r.graphic != 21 || r.scale != 1.0f || r.layer != 0x44) ok = 0;
    if (r.pos.x != 106.0f || r.pos.y != 227.0f) ok = 0;
    r = Draw(s, exprs, 2, 1, 0, 0.9f);
    if (!r.drawn || r.graphic != 9 || r.scale != 0.9f * 0.33333334f || r.layer != 0x45 || r.button != 13) ok = 0;
    if (Draw(s, exprs, 0, 3, 0, 1.0f).drawn || Draw(s, exprs, 0, -1, 0, 1.0f).drawn) ok = 0;
    Setup d = s; d.dead = true;
    if (Draw(d, exprs, 0, 0, 0, 1.0f).drawn) ok = 0;
    Setup n = s; n.hasComponent = false;
    if (Draw(n, exprs, 0, 0, 0, 1.0f).drawn) ok = 0;
    std::printf(ok ? "00658020_TEST PASS\n" : "00658020_TEST FAIL\n");
    return ok ? 0 : 1;
}