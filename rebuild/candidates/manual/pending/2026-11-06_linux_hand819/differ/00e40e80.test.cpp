// Behaviour test for CV_BordelloScript::CMagicman::Main (0x00E40E80): the beer reactions by BeersDrunk (lines BEER0..BEER5, the
// drunk / quickie cut scene at the sixth and the party flag, then BEERTOOMUCH and the tankard handed back), the chatter counter
// (lines 1..4, wrapping) and the one-off pimp-hat remark.
#include <cstdio>

struct Magicman {
    long BeersDrunk, Chat, TankardsReturned; bool HeroFoundDeedsLocation, HeroPartying, MentionedPimpHat;
    const char* LastCutscene; long LastBeerLine;
    void Beer()
    {
        switch (BeersDrunk) {
        case 0: case 1: case 2: case 3: case 4:
            LastBeerLine = BeersDrunk;
            break;
        case 5:
            LastBeerLine = 5;
            if (HeroFoundDeedsLocation) LastCutscene = "QUICKIE";
            else { LastCutscene = "DRUNK"; HeroFoundDeedsLocation = true; }
            HeroPartying = true;
            break;
        default:
            LastBeerLine = -1;
            ++TankardsReturned;
            break;
        }
        ++BeersDrunk;
    }
    long Chatter(bool wearingPimpHat)
    {
        if (!MentionedPimpHat && wearingPimpHat) { MentionedPimpHat = true; return 0; }
        if (Chat > 4) Chat = 1;
        return Chat++;
    }
};

int main()
{
    Magicman m = { 0, 1, 0, false, false, false, 0, -2 };
    bool ok = true;
    for (long i = 0; i < 5; ++i) { m.Beer(); ok = ok && m.LastBeerLine == i && !m.HeroPartying; }
    m.Beer();
    ok = ok && m.LastBeerLine == 5 && m.HeroPartying && m.HeroFoundDeedsLocation && m.LastCutscene[0] == 'D';
    m.Beer();
    ok = ok && m.LastBeerLine == -1 && m.TankardsReturned == 1 && m.BeersDrunk == 7;
    ok = ok && m.Chatter(true) == 0 && m.MentionedPimpHat;
    ok = ok && m.Chatter(true) == 1 && m.Chatter(false) == 2 && m.Chatter(false) == 3 && m.Chatter(false) == 4 && m.Chatter(false) == 1;
    if (!ok) { std::printf("00e40e80_TEST FAIL\n"); return 1; }
    std::printf("00e40e80_TEST PASS\n");
    return 0;
}
