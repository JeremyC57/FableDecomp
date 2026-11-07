// Behaviour test for 0x00DD9A50 NScript::CQ_PrisonEscapeRescueMotherScript::ResetCells: models the decisions the row makes
// (torture branch vs key-stolen teleport, the year counter capped at 5 with an age step only while it grows, the hairstyle table
// per year, the tortured-before latch, and the closing scene chosen by key / race won / race number).
#include <stdio.h>
#include <string.h>

struct Master { bool KeyStolen; bool TorturedBefore; bool RaceWon; long RaceNumber; };
struct State {
    long YearsPassed; bool FoundWeapons; bool CutsceneFinished;
    float Age; int Hair[3]; int HairCount; bool Removed; bool Confiscated;
    const char* Torture2; const char* Closing; bool Teleported;
};

static void Reset(State& s) { memset(&s, 0, sizeof(s)); s.FoundWeapons = true; s.Age = 20.0f; }

// hairstyle ids per year (1..4), 0 = remove; ids stand for the OBJECT_HERO_* names in table order
static void Hairstyle(State& s)
{
    static const int table[5][3] = { { 0, 0, 0 }, { 1, 2, 0 }, { 3, 4, 5 }, { 6, 7, 0 }, { 8, 9, 0 } };
    switch (s.YearsPassed) {
    case 0: s.Removed = true; break;
    case 1: case 2: case 3: case 4:
        for (int i = 0; i < 3 && table[s.YearsPassed][i]; i++) s.Hair[s.HairCount++] = table[s.YearsPassed][i];
        break;
    }
}

static void ResetCells(State& s, Master& m)
{
    if (!m.KeyStolen) {
        if (++s.YearsPassed <= 5) s.Age += 1.0f;
        else s.YearsPassed = 5;
        Hairstyle(s);
        s.FoundWeapons = false;
        s.Confiscated = true;
        if (m.TorturedBefore) s.Torture2 = "CS_PRISON_TORTURE_2_SHORT";
        else { s.Torture2 = "CS_PRISON_TORTURE_2"; m.TorturedBefore = true; }
    } else
        s.Teleported = true;
    if (m.KeyStolen) s.Closing = "key";
    else if (m.RaceWon) s.Closing = "won";
    else if (m.RaceNumber > 1) s.Closing = "again";
    else s.Closing = "first";
    s.CutsceneFinished = true;
}

int main()
{
    int ok = 1;
    State s; Master m = { false, false, false, 0 };
    Reset(s); ResetCells(s, m);
    ok &= s.YearsPassed == 1 && s.Age == 21.0f && s.HairCount == 2 && s.Hair[0] == 1 && !s.FoundWeapons && s.Confiscated;
    ok &= strcmp(s.Torture2, "CS_PRISON_TORTURE_2") == 0 && m.TorturedBefore && strcmp(s.Closing, "first") == 0 && s.CutsceneFinished;
    s.HairCount = 0; ResetCells(s, m);
    ok &= s.YearsPassed == 2 && s.HairCount == 3 && strcmp(s.Torture2, "CS_PRISON_TORTURE_2_SHORT") == 0;
    s.YearsPassed = 5; s.HairCount = 0; s.Age = 30.0f; ResetCells(s, m);
    ok &= s.YearsPassed == 5 && s.Age == 30.0f && s.HairCount == 0 && !s.Removed;
    m.RaceWon = true; Reset(s); ResetCells(s, m); ok &= strcmp(s.Closing, "won") == 0;
    m.RaceWon = false; m.RaceNumber = 2; Reset(s); ResetCells(s, m); ok &= strcmp(s.Closing, "again") == 0;
    m.KeyStolen = true; Reset(s); ResetCells(s, m);
    ok &= s.Teleported && s.YearsPassed == 0 && s.FoundWeapons && strcmp(s.Closing, "key") == 0;
    if (ok) printf("00dd9a50_TEST PASS\n"); else printf("00dd9a50_TEST FAIL\n");
    return ok ? 0 : 1;
}