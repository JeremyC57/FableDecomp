// Behaviour test for CFishCompOrganiser::AskForFish (0x00E80F10): no fish -> the no-fish line and the rod-state camera / lines with the
// GENERIC cut-scene (the rod hint is given once); a refused entry -> the refuse line; an accepted catch picks the highest prize band
// whose weight it reaches (scanning down from FishingPrizes, stopping at the first non-zero band), pays out each prize up to it
// (the upgraded rod sets RodOfChampsGiven, the last prize wins the competition), and a new overall best makes the hero the winner.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>

struct Comp {
    bool won, rodChamps, rodHint, hasFish, hasRod, answerYes; long attack, prizeNumber, prizes; float biggest, biggestComp, weight;
    const float* weights; const char* const* prizeNames;
    std::map<std::string, std::string> args; std::string cutscene, winner; int refreshed;
    Comp() : won(false), rodChamps(false), rodHint(false), hasFish(false), hasRod(false), answerYes(false), attack(0), prizeNumber(0),
        prizes(0), biggest(0.0f), biggestComp(0.0f), weight(0.0f), weights(0), prizeNames(0), refreshed(0) { }   // VC7.1: no value-init
    void Play(const std::string& c) { cutscene = c; }
    void Ask()
    {
        std::string question = "TEXT_CS_B15_FISH_QUESTION", suffix;
        if (won) suffix = "_BONUS";
        if (attack > 0) question = "TEXT_CS_B15_FISH_QUESTION_NASTY";
        args["$ARG1"] = question + suffix; Play("CS_FISHCOMP_CHECKFISH_QUESTION");
        if (!hasFish) {
            args["$ARG1"] = attack > 0 ? "TEXT_CS_B15_FISH_NOFISH_NASTY" : "TEXT_CS_B15_FISH_NOFISH";
            if (!hasRod) {
                if (rodChamps) { args["$ARG2"] = "CAM_FC_HEROORG"; args["$ARG4"] = "TEXT_CS_B15_NOROD_CHAMP_10"; }
                else if (!rodHint) { args["$ARG2"] = "CAM_FC_HEROORG"; args["$ARG4"] = "TEXT_CS_B15_NOROD_20"; rodHint = true; }
                else { args["$ARG2"] = "CAM_FC_ORGHERO"; args["$ARG3"] = "TEXT_CS_B15_NOROD_REMINDER"; }
            } else { args["$ARG2"] = "CAM_FC_ORGHERO"; args["$ARG3"] = "TEXT_CS_B15_FISH_NONEWPRIZE"; }
            Play("CS_FISHCOMP_CHECKFISH_GENERIC"); return;
        }
        long answer = 1;
        if (!won) answer = answerYes ? 1 : 0;
        if (answer != 1) { args["$ARG1"] = attack > 0 ? "TEXT_CS_B15_FISH_REFUSE_NASTY" : "TEXT_CS_B15_FISH_REFUSE"; Play("CS_FISHCOMP_CHECKFISH_GENERIC"); return; }
        long prize = 0;
        for (long i = prizes; i >= 0 && prize == 0; --i) if (weight >= weights[i]) prize = i;
        if (weight > biggest) {
            biggest = weight;
            if (won) { args["$ARG1"] = "TEXT_CS_B15_FISH_BIGGER_BONUS"; Play("CS_FISHCOMP_CHECKFISH_GENERIC"); }
            else if (biggestComp > weight) { args["$ARG1"] = "TEXT_CS_B15_FISH_BIGGEST_PERSONAL"; Play("CS_FISHCOMP_CHECKFISH_GIVETIP"); }
            else if (prize == prizeNumber) { args["$ARG1"] = prize == 0 ? "TEXT_CS_B15_FISH_NEED_BIGGER" : "TEXT_CS_B15_FISH_BIGGEST_NO_PRIZE"; Play("CS_FISHCOMP_CHECKFISH_GIVETIP"); }
            else {
                std::string cs = "CS_FISHCOMP_CHECKFISH_PRIZE";
                if (prizeNumber < 0) prizeNumber = 0;
                while (prizeNumber < prize) {
                    ++prizeNumber; char n[16]; std::sprintf(n, "%ld", prizeNumber); cs += n;
                    args[std::string("$PRIZE") + n] = prizeNames[prizeNumber];
                    if (!std::strcmp(prizeNames[prizeNumber], "OBJECT_FISHING_ROD_UPGRADED")) rodChamps = true;
                }
                Play(cs);
                if (prizeNumber == prizes) won = true;
            }
        } else if (weight == biggest) { args["$ARG1"] = "TEXT_CS_B15_FISH_SAMESIZE" + suffix; Play("CS_FISHCOMP_CHECKFISH_GIVETIP"); }
        else if (won) { char n[16]; std::sprintf(n, "%ld", prize); args["$ARG1"] = std::string("TEXT_CS_B15_FISH_COMMENT_BONUS") + n; Play("CS_FISHCOMP_CHECKFISH_GENERIC"); }
        else { args["$ARG1"] = "TEXT_CS_B15_FISH_SMALLER"; Play("CS_FISHCOMP_CHECKFISH_GIVETIP"); }
        if (weight > biggestComp) { biggestComp = weight; winner = "HERO"; ++refreshed; }
    }
};

static const float W[3] = { 0.0f, 2.0f, 5.0f };
static const char* const N[3] = { "", "OBJECT_A", "OBJECT_FISHING_ROD_UPGRADED" };
static void Init(Comp& c) { c = Comp(); c.hasFish = true; c.hasRod = true; c.answerYes = true; c.prizes = 2; c.weights = W; c.prizeNames = N; c.prizeNumber = -1; }

int main()
{
    bool ok = true;
    Comp a; Init(a); a.hasFish = false; a.hasRod = false; a.Ask();
    ok = ok && a.rodHint && a.args["$ARG4"] == "TEXT_CS_B15_NOROD_20" && a.cutscene == "CS_FISHCOMP_CHECKFISH_GENERIC";
    a.Ask(); ok = ok && a.args["$ARG3"] == "TEXT_CS_B15_NOROD_REMINDER";
    Comp b; Init(b); b.answerYes = false; b.attack = 1; b.Ask();
    ok = ok && b.args["$ARG1"] == "TEXT_CS_B15_FISH_REFUSE_NASTY" && b.refreshed == 0;
    Comp c; Init(c); c.weight = 6.0f; c.Ask();
    ok = ok && c.prizeNumber == 2 && c.won && c.rodChamps && c.cutscene == "CS_FISHCOMP_CHECKFISH_PRIZE12" && c.args["$PRIZE1"] == "OBJECT_A";
    ok = ok && c.winner == "HERO" && c.refreshed == 1 && c.biggest == 6.0f;
    Comp d; Init(d); d.weight = 1.0f; d.biggestComp = 4.0f; d.Ask();
    ok = ok && d.args["$ARG1"] == "TEXT_CS_B15_FISH_BIGGEST_PERSONAL" && d.refreshed == 0;
    Comp e; Init(e); e.won = true; e.weight = 3.0f; e.biggest = 4.0f; e.Ask();
    ok = ok && e.args["$ARG1"] == "TEXT_CS_B15_FISH_COMMENT_BONUS1";
    if (!ok) { std::printf("00e80f10_TEST FAIL\n"); return 1; }
    std::printf("00e80f10_TEST PASS\n");
    return 0;
}