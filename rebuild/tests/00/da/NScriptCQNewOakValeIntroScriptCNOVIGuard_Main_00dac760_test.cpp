// Behaviour test for CNOVI_Guard::Main (0x00DAC760): the guard notices the hero once his bad deeds outnumber those already dealt with
// and he is inside the deed-scaled range {0, 5, 10, 15}[min(bad, 3)]; the first arrest reads the full charge sheet (one line per crime
// flag), later ones the "again" sheet; dealt-with catches up with the bad-deed count; talk replies good / bad / neutral.
#include <cstdio>
#include <cstring>

static const float NoticeDistances[4] = { 0.0f, 5.0f, 10.0f, 15.0f };
struct Guard {
    long Good, Bad, Dealt; bool SpokenOnce; bool Crimes[5]; long Lines, FullSheets, AgainSheets;
    void Step(float heroDistance)
    {
        if (Bad - Dealt > 0 && heroDistance < NoticeDistances[Bad < 3 ? Bad : 3]) {
            Dealt = Bad;
            if (!SpokenOnce) { ++FullSheets; SpokenOnce = true; } else ++AgainSheets;
            for (int i = 0; i < 5; ++i) if (Crimes[i]) ++Lines;
        }
    }
    const char* Talk() const
    {
        if (Bad == 0 && Good > 0) return "TEXT_QST_048_GUARD_ON_TALK_GOOD";
        if (Bad > 0) return "TEXT_QST_048_GUARD_ON_TALK_BAD";
        return "TEXT_QST_048_GUARD_ON_TALK_NEUTRAL";
    }
};

int main()
{
    Guard g = { 1, 0, 0, false, { false, false, false, false, false }, 0, 0, 0 };
    bool ok = std::strcmp(g.Talk(), "TEXT_QST_048_GUARD_ON_TALK_GOOD") == 0;
    g.Step(1.0f); ok = ok && g.FullSheets == 0;                       // no bad deeds: nothing to deal with
    g.Bad = 1; g.Crimes[0] = true;
    g.Step(6.0f); ok = ok && g.FullSheets == 0;                       // one deed: notice range 5
    g.Step(4.0f); ok = ok && g.FullSheets == 1 && g.Dealt == 1 && g.Lines == 1;
    g.Step(4.0f); ok = ok && g.FullSheets == 1 && g.AgainSheets == 0; // already dealt with
    g.Bad = 4; g.Crimes[2] = true;
    g.Step(14.0f); ok = ok && g.AgainSheets == 1 && g.Dealt == 4 && g.Lines == 3;   // capped index 3: range 15
    ok = ok && std::strcmp(g.Talk(), "TEXT_QST_048_GUARD_ON_TALK_BAD") == 0;
    g.Good = 0; g.Bad = 0; ok = ok && std::strcmp(g.Talk(), "TEXT_QST_048_GUARD_ON_TALK_NEUTRAL") == 0;
    if (!ok) { std::printf("00dac760_TEST FAIL\n"); return 1; }
    std::printf("00dac760_TEST PASS\n");
    return 0;
}