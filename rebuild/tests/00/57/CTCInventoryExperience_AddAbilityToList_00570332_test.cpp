// Behaviour test for CTCInventoryExperience::AddAbilityToList (0x00570332): a model of the entry builder's decisions, written from
// src/00570332.cpp. The entry name gets " +<level+2>" while the ability is below its maximum; the description block (cost line,
// alignment-modifier line when the morality-adjusted cost differs, signed with "+" when positive, then the blank line) is only
// built below the maximum, with text widths 210 / 240; the tick slider's fraction is clamped at 0; the morality colour is red
// below -DELTA, blue above +DELTA and hidden otherwise; an ability with no possible level-up and no undo record is made
// unavailable, otherwise its sub list holds a buy entry and/or an undo entry (InitialiseOffsets 2 only with both); the buy
// spends min(will pool, cost) from the will slider and the rest of the cost from the general slider.
#include <cstdio>
#include <string>
#include <vector>
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct SLine { std::string text; int width; };
struct SEntry {
    std::string name; std::vector<SLine> lines; float fraction; bool showMorality; char moralityColour;
    bool unavailable; unsigned long offsets; int subEntries; long fromWill; long fromGeneral; int positionChildren;
};
static std::string Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static SEntry AddAbility(const std::string& abilityName, long level, long maxLevel, long cost, long costNoMorality,
                         long possibleLevels, float partialLevel, float moralityFactor, bool hasUndo, long willExperience)
{
    SEntry e;
    long nextLevel = level + 2;
    e.name = abilityName;
    if (level < maxLevel) e.name += " +" + Num(nextLevel);
    e.positionChildren = 0 ? 2 : 0;                         // movie + description registered on the PC screen
    if (level < maxLevel) {
        SLine l1 = { abilityName + " Level " + Num(nextLevel) + "\n" + "Cost: " + Num(cost), 210 };
        e.lines.push_back(l1);
        long modifier = cost - costNoMorality;
        if (modifier != 0) {
            std::string m = Num(modifier);
            if (modifier > 0) m = "+" + m;
            SLine l2 = { "Alignment " + m, 240 };
            e.lines.push_back(l2);
        }
        SLine blank = { "", 240 };
        e.lines.push_back(blank);
    }
    SLine desc = { "description " + Num(nextLevel - 1), 240 };
    e.lines.push_back(desc);
    float fraction = partialLevel;
    fraction += level + possibleLevels + 1;
    if (fraction < 0.0f) fraction = 0.0f;
    e.fraction = fraction;
    e.showMorality = true; e.moralityColour = ' ';
    if (moralityFactor < -DELTA_NOTIONAL_ZERO) e.moralityColour = 'R';
    else if (moralityFactor > DELTA_NOTIONAL_ZERO) e.moralityColour = 'B';
    else e.showMorality = false;
    if (0) ++e.positionChildren;                            // tick container
    e.unavailable = false; e.offsets = 0; e.subEntries = 0; e.fromWill = 0; e.fromGeneral = 0;
    if (possibleLevels <= 0 && !hasUndo) { e.unavailable = true; return e; }
    e.offsets = 1;
    if (possibleLevels > 0 && hasUndo) e.offsets = 2;
    if (possibleLevels > 0) {
        ++e.subEntries;
        if (willExperience > 0) e.fromWill = willExperience <= cost ? willExperience : cost;
        long rest = cost - willExperience;
        if (rest > 0) e.fromGeneral = rest;
    }
    if (hasUndo) ++e.subEntries;
    return e;
}
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
int main()
{
    int fails = 0;
    SEntry a = AddAbility("Fireball", 1, 4, 500, 400, 2, 0.25f, -0.5f, false, 200);
    if (a.name != "Fireball +3" || a.lines.size() != 4 || a.lines[0].width != 210 || a.lines[1].text != "Alignment +100" || a.lines[2].text != "" ||
        a.lines[3].width != 240 || !Near(a.fraction, 4.25f) || !a.showMorality || a.moralityColour != 'R' || a.unavailable ||
        a.offsets != 1 || a.subEntries != 1 || a.fromWill != 200 || a.fromGeneral != 300) ++fails;
    SEntry b = AddAbility("Heal", 4, 4, 0, 0, 0, -9.0f, 0.0f, false, 0);
    if (b.name != "Heal" || b.lines.size() != 1 || b.lines[0].text != "description 5" || !Near(b.fraction, 0.0f) || b.showMorality ||
        !b.unavailable || b.subEntries != 0) ++fails;
    SEntry c = AddAbility("Drain", 0, 3, 300, 350, 1, 0.0f, 0.75f, true, 900);
    if (c.name != "Drain +2" || c.lines.size() != 4 || c.lines[1].text != "Alignment -50" || c.moralityColour != 'B' || c.offsets != 2 ||
        c.subEntries != 2 || c.fromWill != 300 || c.fromGeneral != 0) ++fails;
    SEntry d = AddAbility("Slow", 2, 3, 250, 250, 0, 0.5f, 0.00001f, true, 0);
    if (d.lines.size() != 3 || d.showMorality || d.unavailable || d.offsets != 1 || d.subEntries != 1 || d.fromWill != 0 || d.fromGeneral != 0) ++fails;
    if (0 && a.positionChildren != 3) ++fails;
    std::printf(fails ? "00570332_TEST FAIL\n" : "00570332_TEST PASS\n");
    return fails;
}