// behaviour test for 0x0094D020 CAIStateGroup_GameMaster::Update (the hands-off mode toggle and the offer/base-behaviour loop)
#include <cstdio>
struct Group {
    bool HandsOffMode; int adds, removes, offers, bases, terminates;
    int gameSteps; const bool* inProgress; bool heroTargetsMe; int offerResults; int baseResults;
    bool OfferPlayerAGame() { offers++; return offerResults-- > 0; }
    bool v15() { bases++; return baseResults-- > 0; }
    void Loop()
    {
        bool result;
        int step = 0;
        do {
            if (step >= gameSteps) break;
            bool game = inProgress[step++];
            if (game) {
                if (!HandsOffMode) { HandsOffMode = true; adds++; }
            } else if (HandsOffMode) {
                HandsOffMode = false; removes++;
            }
            if (!game) {
                if (heroTargetsMe) { result = OfferPlayerAGame(); continue; }
            }
            result = v15();
        } while (result);
        terminates++;
    }
};
int main()
{
    const bool seq[] = { true, true, false, false };
    Group g = { false, 0, 0, 0, 0, 0, 4, seq, true, 5, 5 };
    g.Loop();
    bool ok = g.adds == 1 && g.removes == 1 && g.offers == 2 && g.bases == 2 && g.terminates == 1 && !g.HandsOffMode;
    Group h = { false, 0, 0, 0, 0, 0, 4, seq, false, 0, 0 };
    h.Loop();
    ok = ok && h.bases == 1 && h.offers == 0 && h.adds == 1 && h.terminates == 1;
    std::printf(ok ? "0094d020_TEST PASS\n" : "0094d020_TEST FAIL\n");
    return ok ? 0 : 1;
}