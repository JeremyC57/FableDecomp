// Behaviour test for 0x00EB40C0 CMurderTwistAssassin::DoWatchingForTrouble: a standalone model of the decision flow
// (talked to -> intro lines once, then cycling start lines 1..2; then, separately, first threat -> threatened line + sheathe,
// else hit / special ability other than heal-life / threatened again -> deal with the hero).
#include <cstdio>
#include <cstring>

struct Model {
    bool talkedTo, hitBy, hitByAnyAbility, hitByHealLife, threatened, alive;
    bool hasGivenIntro, hasBeenThreatened;
    long nextLine, assassinState;
    int spoken; char lastLine[64];
    bool sheathed, dealtWith;
};

static void Speak(Model& m, const char* line)
{
    if (!m.alive) return;
    ++m.spoken;
    std::strcpy(m.lastLine, line);
}

static void DoWatchingForTrouble(Model& m)
{
    if (m.talkedTo) {
        if (m.hasGivenIntro) {
            if (m.nextLine > 2) m.nextLine = 1;
            char text[64];
            std::sprintf(text, "TEXT_QST_B07_ASSASSIN_START%ld", m.nextLine);
            Speak(m, text);
            m.nextLine++;
        } else {
            Speak(m, "TEXT_QST_B07_ASSASSIN_INTRO");
            Speak(m, "TEXT_QST_B07_ASSASSIN_INTRO_FINISHED");
            m.assassinState = 1;
            m.hasGivenIntro = true;
        }
    }
    if (!m.hasBeenThreatened && m.threatened) {
        Speak(m, "TEXT_QST_B07_ASSASSIN_THREATENED");
        m.sheathed = true;
        m.hasBeenThreatened = true;
    } else if (m.hitBy || (m.hitByAnyAbility && !m.hitByHealLife) || (m.hasBeenThreatened && m.threatened)) {
        m.dealtWith = true;
    }
}

int main()
{
    int fails = 0;
    Model m; std::memset(&m, 0, sizeof m); m.alive = true; m.nextLine = 1;
    m.talkedTo = true;
    DoWatchingForTrouble(m);
    if (m.spoken != 2 || std::strcmp(m.lastLine, "TEXT_QST_B07_ASSASSIN_INTRO_FINISHED") || !m.hasGivenIntro || m.assassinState != 1) ++fails;
    DoWatchingForTrouble(m); DoWatchingForTrouble(m);
    if (std::strcmp(m.lastLine, "TEXT_QST_B07_ASSASSIN_START2") || m.nextLine != 3) ++fails;
    DoWatchingForTrouble(m);
    if (std::strcmp(m.lastLine, "TEXT_QST_B07_ASSASSIN_START1") || m.nextLine != 2) ++fails;
    // talk arm falls through into the threat test
    m.threatened = true; m.spoken = 0;
    DoWatchingForTrouble(m);
    if (m.spoken != 2 || !m.sheathed || !m.hasBeenThreatened || m.dealtWith) ++fails;
    m.talkedTo = false;
    DoWatchingForTrouble(m);
    if (!m.dealtWith) ++fails;
    Model h; std::memset(&h, 0, sizeof h); h.alive = true;
    h.hitByAnyAbility = true; h.hitByHealLife = true;
    DoWatchingForTrouble(h);
    if (h.dealtWith) ++fails;
    h.hitByHealLife = false;
    DoWatchingForTrouble(h);
    if (!h.dealtWith) ++fails;
    if (fails) { std::printf("00eb40c0_TEST FAIL\n"); return 1; }
    std::printf("00eb40c0_TEST PASS\n");
    return 0;
}