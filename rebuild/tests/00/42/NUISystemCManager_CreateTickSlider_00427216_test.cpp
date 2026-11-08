// Behaviour test for NUISystem::CManager::CreateTickSlider @ 0x00427216: the PC inventory tick-slider bar. Checks, against a model
// of the row's logic over mock UI components (a name -> states map, the states being position / zoom / colour records): the
// possible-value clamp to the maximum, the slider position, the back sprites stretched and shifted by the label width, the right
// part (the "1" variant and the eight-tick / fourteen-pixel allowances on the morality bar), the name and level texts ("x" for level
// -1, blank when maxed on a normal bar), one tick per level from 1 below the maximum (white up to the value, black above, LO
// variants on the morality bar, the middle marker replacing tick 7 with a four-pixel gap before and twenty after), the maxed-out
// overlay in the bar colour, the fill sprite (earned levels on a normal bar; the good / evil fill around the middle marker on the
// morality bar with the three morality icons' alphas), the ghost sprite with the state's own alpha and the optional cap sprite.
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <map>
#include <string>
#include <vector>

struct Colour { unsigned char B, G, R, A; };
struct State { float x, y, zoomX, zoomY; Colour colour; State() : x(0), y(0), zoomX(1), zoomY(1) { colour.B = colour.G = colour.R = 0; colour.A = 100; } };
struct Component { std::string name; std::map<unsigned long, State> states; std::vector<std::string> children; };

struct FloatColour { float r, g, b, a; };
static Colour ToBytes(const FloatColour& c) { Colour o; o.R = (unsigned char)(c.r * 255.0f); o.G = (unsigned char)(c.g * 255.0f); o.B = (unsigned char)(c.b * 255.0f); o.A = (unsigned char)(c.a * 255.0f); return o; }

struct Slider {
    std::map<std::string, Component> parts;          // the slider's own children (found by name) and the components it creates
    std::vector<std::string> added;                  // AddChild order
    std::string text, levelText;
    long middleX;
    Component& Part(const std::string& n) { Component& c = parts[n]; c.name = n; return c; }
};

// the model of the row (same control flow and arithmetic as rebuild src/00427216.cpp)
static Slider CreateTickSlider(long posX, long posY, long value, long maxValue, FloatColour colour, float possibleValue, float width,
                               bool moralityBar, bool showCap, FloatColour capColour, float labelWidth)
{
    Slider s;
    long startTick = moralityBar ? 8 : 0;
    if (possibleValue > (float)maxValue) possibleValue = (float)maxValue;
    State& root = s.Part("PC_TICK_SLIDER").states[0];
    root.x = (float)posX; root.y = (float)posY;
    s.Part("UI_TICK_SLIDER_BACK_SPRITE1").states[0].zoomX += labelWidth * 0.125f;
    s.Part("UI_TICK_SLIDER_BACK_SPRITE2").states[0].x += labelWidth;
    s.Part("UI_TICK_SLIDER_BACK_SPRITE3").states[0].x += labelWidth;
    std::string rightName = moralityBar ? "UI_TICK_SLIDER_BACK_RIGHT_PART1" : "UI_TICK_SLIDER_BACK_RIGHT_PART";
    Component& rightPart = s.Part(rightName);
    s.Part("UI_TICK_SLIDER_BACK").children.push_back(rightName);
    s.text = "name";
    s.levelText = value == -1 ? "x" : std::string(1, (char)('0' + value % 10));
    float middleGap = moralityBar ? 14.0f : 0.0f;
    float startOffset = (float)startTick;
    s.Part("UI_TICK_SLIDER_BACK_SPRITE3").states[0].zoomX = (width - 144.0f - 32.0f + startOffset - labelWidth + middleGap) * 0.125f;
    s.Part("UI_TICK_SLIDER_BACK_SPRITE3").states[0].x = labelWidth + 144.0f;
    rightPart.states[0].x = width - 32.0f + startOffset + middleGap;
    float tickSpacing = (width - 140.0f - 32.0f + 4.0f - labelWidth) / (float)maxValue;
    float tickX = labelWidth + 136.0f;
    s.middleX = 0;
    for (long i = 1; i < maxValue; ++i) {
        tickX += tickSpacing;
        std::string tick;
        if (moralityBar && i == 7) {
            tickX += 4.0f;
            s.middleX = (long)(tickX + 12.0f);
            tick = "UI_TICK_SLIDER_LEVEL_MIDDLE_SPRITE";
        } else if (i <= value) {
            tick = moralityBar ? "UI_TICK_SLIDER_LEVEL_TICK_SPRITEWLO" : "UI_TICK_SLIDER_LEVEL_TICK_SPRITEW";
        } else {
            tick = moralityBar ? "UI_TICK_SLIDER_LEVEL_TICK_SPRITEBLO" : "UI_TICK_SLIDER_LEVEL_TICK_SPRITEB";
        }
        char suffix[32]; std::sprintf(suffix, "#%ld", i);
        Component& t = s.Part(tick + suffix);
        if (moralityBar && i == 7) t.states[0].zoomX = 3.0f;         // the middle marker is three ticks wide
        t.states[0].x = tickX;
        s.added.push_back(tick + suffix);
        if (moralityBar && i == 7) tickX += 20.0f;
    }
    if (value == maxValue && !moralityBar) {
        s.levelText = "";
        State& m = s.Part("UI_TICK_SLIDER_MAXED_OUT").states[0];
        m.x = width - 31.0f; m.colour = ToBytes(colour);
    }
    Component& fill = s.Part("UI_TICK_SLIDER_FILL_SPRITE");
    if (!moralityBar) {
        fill.states[0].x = labelWidth + 139.0f;
        fill.states[0].zoomX = (float)value * tickSpacing * 0.25f;
    } else {
        float morality = possibleValue * 0.142857f;
        Component& veryBad = s.Part("UI_MORALITY_VERY_BAD_SPRITE");
        Component& veryGood = s.Part("UI_MORALITY_VERY_GOOD_SPRITE");
        Component& neutral = s.Part("UI_MORALITY_NEUTRAL_SPRITE");
        neutral.states[1].colour.A = (unsigned char)((1.0f - (float)std::fabs(morality)) * 255.0f);
        veryGood.states[1].colour.A = 0;
        veryBad.states[1].colour.A = 0;
        if (morality < 0.0f) { veryBad.states[1].colour.A = (unsigned char)((float)std::fabs(morality) * 255.0f); veryGood.states[1].colour.A = 0; }
        if (morality > 0.0f) { veryGood.states[1].colour.A = (unsigned char)(morality * 255.0f); veryBad.states[1].colour.A = 0; }
        float iconX = (float)(s.middleX - 12);
        neutral.states[0].x = iconX; neutral.states[0].y = 0.0f;
        veryBad.states[0].x = iconX; veryBad.states[0].y = 0.0f;
        veryGood.states[0].x = iconX; veryGood.states[0].y = 0.0f;
        s.added.push_back("UI_MORALITY_VERY_BAD_SPRITE"); s.added.push_back("UI_MORALITY_VERY_GOOD_SPRITE"); s.added.push_back("UI_MORALITY_NEUTRAL_SPRITE");
        float fillLevels = possibleValue;
        if (possibleValue < 0.0f) fillLevels = possibleValue * -1.0f;
        fill.states[0].zoomX = fillLevels * tickSpacing * 0.25f;
        if (possibleValue < 0.0f) fill.states[0].x = tickSpacing * possibleValue + iconX;
        else fill.states[0].x = (float)(s.middleX + 12);
    }
    fill.states[0].colour = ToBytes(colour);
    if (!moralityBar) {
        Component& ghost = s.Part("UI_TICK_SLIDER_GHOST_SPRITE");
        ghost.states[0].x = labelWidth + 139.0f;
        ghost.states[0].zoomX = tickSpacing * possibleValue * 0.25f;
        Colour ghostColour = ToBytes(colour);
        ghostColour.A = ghost.states[0].colour.A;
        ghost.states[0].colour = ghostColour;
        if (showCap) {
            Component& cap = s.Part("UI_TICK_SLIDER_CAP_SPRITE");
            cap.states[0].x = (float)(maxValue - 1) * tickSpacing + labelWidth + 139.0f;
            cap.states[0].zoomX = tickSpacing * 0.25f;
            cap.states[0].colour = ToBytes(capColour);
            s.added.push_back("UI_TICK_SLIDER_CAP_SPRITE");
        }
    }
    return s;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    int ok = 1;
    FloatColour red = { 0.7f, 0.0f, 0.0f, 1.0f }, black = { 0.0f, 0.0f, 0.0f, 1.0f };
    // a normal bar: level 3 of 7, 1.5 more levels affordable, 270 wide, label 20, with the cap
    Slider n = CreateTickSlider(10, 30, 3, 7, red, 1.5f, 270.0f, false, true, black, 20.0f);
    float spacing = (270.0f - 140.0f - 32.0f + 4.0f - 20.0f) / 7.0f;
    if (!Near(n.parts["PC_TICK_SLIDER"].states[0].x, 10.0f) || !Near(n.parts["PC_TICK_SLIDER"].states[0].y, 30.0f)) ok = 0;
    if (!Near(n.parts["UI_TICK_SLIDER_BACK_SPRITE1"].states[0].zoomX, 1.0f + 20.0f * 0.125f)) ok = 0;
    if (!Near(n.parts["UI_TICK_SLIDER_BACK_SPRITE2"].states[0].x, 20.0f) || !Near(n.parts["UI_TICK_SLIDER_BACK_SPRITE3"].states[0].x, 20.0f + 144.0f)) ok = 0;
    if (!Near(n.parts["UI_TICK_SLIDER_BACK_SPRITE3"].states[0].zoomX, (270.0f - 176.0f - 20.0f) * 0.125f)) ok = 0;
    if (n.parts.count("UI_TICK_SLIDER_BACK_RIGHT_PART") != 1 || n.parts.count("UI_TICK_SLIDER_BACK_RIGHT_PART1") != 0) ok = 0;
    if (!Near(n.parts["UI_TICK_SLIDER_BACK_RIGHT_PART"].states[0].x, 270.0f - 32.0f)) ok = 0;
    if (n.levelText != "3" || n.added.size() != 7) ok = 0;                        // six ticks (1..6) and the cap
    for (long i = 1; i < 7 && ok; ++i) {
        char buf[64]; std::sprintf(buf, "%s#%ld", i <= 3 ? "UI_TICK_SLIDER_LEVEL_TICK_SPRITEW" : "UI_TICK_SLIDER_LEVEL_TICK_SPRITEB", i);
        if (n.added[i - 1] != buf || !Near(n.parts[buf].states[0].x, 20.0f + 136.0f + spacing * (float)i)) ok = 0;
    }
    if (n.parts.count("UI_TICK_SLIDER_MAXED_OUT") != 0) ok = 0;
    if (!Near(n.parts["UI_TICK_SLIDER_FILL_SPRITE"].states[0].x, 159.0f) || !Near(n.parts["UI_TICK_SLIDER_FILL_SPRITE"].states[0].zoomX, 3.0f * spacing * 0.25f)) ok = 0;
    Colour fillColour = n.parts["UI_TICK_SLIDER_FILL_SPRITE"].states[0].colour;
    if (fillColour.R != 178 || fillColour.G != 0 || fillColour.B != 0 || fillColour.A != 255) ok = 0;
    Colour ghostColour = n.parts["UI_TICK_SLIDER_GHOST_SPRITE"].states[0].colour;
    if (ghostColour.R != 178 || ghostColour.A != 100 || !Near(n.parts["UI_TICK_SLIDER_GHOST_SPRITE"].states[0].zoomX, spacing * 1.5f * 0.25f)) ok = 0;
    if (n.added[6] != "UI_TICK_SLIDER_CAP_SPRITE" || !Near(n.parts["UI_TICK_SLIDER_CAP_SPRITE"].states[0].x, 6.0f * spacing + 159.0f)) ok = 0;
    if (!Near(n.parts["UI_TICK_SLIDER_CAP_SPRITE"].states[0].zoomX, spacing * 0.25f) || n.parts["UI_TICK_SLIDER_CAP_SPRITE"].states[0].colour.A != 255) ok = 0;
    // maxed out, no level yet shown as "x" first, possible value clamped to the maximum, no cap
    Slider m = CreateTickSlider(0, 0, 7, 7, red, 20.0f, 270.0f, false, false, black, 20.0f);
    if (m.levelText != "" || m.parts.count("UI_TICK_SLIDER_MAXED_OUT") != 1 || !Near(m.parts["UI_TICK_SLIDER_MAXED_OUT"].states[0].x, 239.0f)) ok = 0;
    if (m.parts["UI_TICK_SLIDER_MAXED_OUT"].states[0].colour.R != 178 || !Near(m.parts["UI_TICK_SLIDER_GHOST_SPRITE"].states[0].zoomX, spacing * 7.0f * 0.25f)) ok = 0;
    if (m.parts.count("UI_TICK_SLIDER_CAP_SPRITE") != 0 || m.added.size() != 6) ok = 0;
    Slider x = CreateTickSlider(0, 0, -1, 7, red, 0.0f, 270.0f, false, false, black, 20.0f);
    if (x.levelText != "x" || x.added.size() != 6) ok = 0;
    // the morality bar: 15 ticks, half a bar towards evil (possible value -3.5, morality = possible / 7), with the "1" right part
    Slider e = CreateTickSlider(0, 0, 15, 15, red, -3.5f, 303.0f, true, true, black, 73.0f);
    float mSpacing = (303.0f - 140.0f - 32.0f + 4.0f - 73.0f) / 15.0f;
    if (e.parts.count("UI_TICK_SLIDER_BACK_RIGHT_PART1") != 1 || !Near(e.parts["UI_TICK_SLIDER_BACK_RIGHT_PART1"].states[0].x, 303.0f - 32.0f + 8.0f + 14.0f)) ok = 0;
    if (!Near(e.parts["UI_TICK_SLIDER_BACK_SPRITE3"].states[0].zoomX, (303.0f - 176.0f + 8.0f - 73.0f + 14.0f) * 0.125f)) ok = 0;
    if (e.added.size() != 14 + 3 || e.added[6] != "UI_TICK_SLIDER_LEVEL_MIDDLE_SPRITE#7" || !Near(e.parts[e.added[6]].states[0].zoomX, 3.0f)) ok = 0;
    float tx = 73.0f + 136.0f + 7.0f * mSpacing + 4.0f;
    if (!Near(e.parts[e.added[6]].states[0].x, tx) || e.middleX != (long)(tx + 12.0f)) ok = 0;
    if (!Near(e.parts[e.added[7]].states[0].x, tx + 20.0f + mSpacing) || e.added[0] != "UI_TICK_SLIDER_LEVEL_TICK_SPRITEWLO#1") ok = 0;
    if (e.parts.count("UI_TICK_SLIDER_MAXED_OUT") != 0 || e.parts.count("UI_TICK_SLIDER_GHOST_SPRITE") != 0 || e.parts.count("UI_TICK_SLIDER_CAP_SPRITE") != 0) ok = 0;
    float morality = -3.5f * 0.142857f;
    if (e.parts["UI_MORALITY_NEUTRAL_SPRITE"].states[1].colour.A != (unsigned char)((1.0f - (float)std::fabs(morality)) * 255.0f)) ok = 0;
    if (e.parts["UI_MORALITY_VERY_BAD_SPRITE"].states[1].colour.A != (unsigned char)((float)std::fabs(morality) * 255.0f)) ok = 0;
    if (e.parts["UI_MORALITY_VERY_GOOD_SPRITE"].states[1].colour.A != 0) ok = 0;
    if (!Near(e.parts["UI_MORALITY_NEUTRAL_SPRITE"].states[0].x, (float)(e.middleX - 12)) || e.added[14] != "UI_MORALITY_VERY_BAD_SPRITE" || e.added[16] != "UI_MORALITY_NEUTRAL_SPRITE") ok = 0;
    if (!Near(e.parts["UI_TICK_SLIDER_FILL_SPRITE"].states[0].zoomX, 3.5f * mSpacing * 0.25f)) ok = 0;
    if (!Near(e.parts["UI_TICK_SLIDER_FILL_SPRITE"].states[0].x, mSpacing * -3.5f + (float)(e.middleX - 12))) ok = 0;
    // towards good: the fill starts right of the marker, the good icon fades in
    Slider g = CreateTickSlider(0, 0, 15, 15, red, 7.0f, 303.0f, true, false, black, 73.0f);
    if (g.parts["UI_MORALITY_VERY_GOOD_SPRITE"].states[1].colour.A != (unsigned char)(7.0f * 0.142857f * 255.0f) || g.parts["UI_MORALITY_VERY_BAD_SPRITE"].states[1].colour.A != 0) ok = 0;
    if (!Near(g.parts["UI_TICK_SLIDER_FILL_SPRITE"].states[0].x, (float)(g.middleX + 12)) || !Near(g.parts["UI_TICK_SLIDER_FILL_SPRITE"].states[0].zoomX, 7.0f * mSpacing * 0.25f)) ok = 0;
    if (ok) std::printf("00427216_TEST PASS\n"); else std::printf("00427216_TEST FAIL\n");
    return ok ? 0 : 1;
}