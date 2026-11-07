// Behaviour test for CV_IntroductionToTrophiesScript::CBriarRose::Main (0x00E9E420): each frame the opinion source switches to the
// required loudness (0 LOUD, 1 MEDIUM, 2 QUIET) only when it differs from the current one; the hit warning is given once, after
// the attack speech, when the hero's hit count exceeds TrophiesHitCountForBriarAttack; an expired anim timer plays idle rand()%8 and
// rearms to rand()%Var + Base; an expired opinion-deed timer posts SHOW_TROPHY_GOOD and rearms.
#include <cstdio>

struct Briar {
    long OpinionCurrent, OpinionRequired, Switches; const char* Source;
    long HitByHeroCount, HitLimit; bool AttackSpeechComplete, HitWarningComplete; long Warnings;
    long AnimTimer, AnimBase, AnimVar, LastAnim, DeedTimer, DeedPeriod, Deeds;
    void Frame(long rnd)
    {
        if (OpinionCurrent != OpinionRequired) {
            if (OpinionRequired == 0) { Source = "LOUD"; OpinionCurrent = 0; ++Switches; }
            else if (OpinionRequired == 1) { Source = "MEDIUM"; OpinionCurrent = 1; ++Switches; }
            else if (OpinionRequired == 2) { Source = "QUIET"; OpinionCurrent = 2; ++Switches; }
        }
        if (HitByHeroCount > HitLimit && AttackSpeechComplete && !HitWarningComplete) { ++Warnings; HitWarningComplete = true; }
        if (AnimTimer == 0) { LastAnim = rnd % 8; AnimTimer = rnd % AnimVar + AnimBase; } else --AnimTimer;
        if (DeedTimer <= 0) { ++Deeds; DeedTimer = DeedPeriod; } else --DeedTimer;
    }
};

int main()
{
    bool ok = true;
    Briar b = { 0, 0, 0, "LOUD", 0, 3, false, false, 0, 0, 10, 5, -1, 1, 1, 0 };
    b.Frame(13); ok = ok && b.Switches == 0 && b.LastAnim == 5 && b.AnimTimer == 13 && b.Deeds == 0;
    b.OpinionRequired = 2; b.Frame(0); ok = ok && b.Switches == 1 && b.Source[0] == 'Q' && b.OpinionCurrent == 2;
    b.Frame(0); ok = ok && b.Switches == 1 && b.Deeds == 1;
    b.OpinionRequired = 1; b.Frame(0); ok = ok && b.Source[0] == 'M';
    b.OpinionRequired = 7; b.Frame(0); ok = ok && b.Switches == 2 && b.OpinionCurrent == 1;  // unknown level: no switch
    b.HitByHeroCount = 4; b.Frame(0); ok = ok && b.Warnings == 0;                            // no attack speech yet
    b.AttackSpeechComplete = true; b.HitByHeroCount = 3; b.Frame(0); ok = ok && b.Warnings == 0; // not above the limit
    b.HitByHeroCount = 4; b.Frame(0); b.Frame(0); ok = ok && b.Warnings == 1;                // once only
    if (!ok) { std::printf("00e9e420_TEST FAIL\n"); return 1; }
    std::printf("00e9e420_TEST PASS\n");
    return 0;
}
