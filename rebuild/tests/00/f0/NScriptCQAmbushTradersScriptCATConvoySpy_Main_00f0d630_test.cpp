// Behaviour test for CAT_ConvoySpy::Main (0x00F0D630): the frame step of the convoy spy -- follow the hero once seen (re-armed when
// close), stop at the no-entry marker until the hero reaches the reset marker, play the Bowerstone Posh outro once (Tanya / no-Tanya
// cut scene by the contact's fate), and obey the FOLLOW / WAIT expressions (FOLLOW also makes contact once).
#include <cstdio>
#include <cstring>

struct Spy {
    bool Seen, Proximity, WanderReset, ContactMade, AtNoEntry, SpeechComplete, Following, ContactKilled;
    long Follows, Outros, TanyaOutros, Extractions;
    void Step(bool heroClose, bool atNoEntry, bool heroAtReset, bool posh, const char* expression)
    {
        if ((Seen || Proximity) && !ContactMade && !AtNoEntry) {
            if (WanderReset) { ++Follows; WanderReset = false; }
            if (heroClose) WanderReset = true;
        }
        if (!ContactMade) {
            if (atNoEntry && !AtNoEntry) AtNoEntry = true;
            else if (heroAtReset && AtNoEntry) { ++Follows; AtNoEntry = false; }
        }
        if (posh && !SpeechComplete) { ++Outros; if (!ContactKilled) ++TanyaOutros; SpeechComplete = true; }
        if (expression) {
            if (std::strcmp(expression, "EXPRESSION_FOLLOW") == 0) {
                if (!ContactMade) { ContactMade = true; ++Extractions; }
                Following = true;
            }
            else if (std::strcmp(expression, "EXPRESSION_WAIT") == 0) Following = false;
        }
    }
};

int main()
{
    Spy s = { true, false, true, false, false, false, false, false, 0, 0, 0, 0 };
    bool ok = true;
    s.Step(false, false, false, false, 0);  ok = ok && s.Follows == 1 && !s.WanderReset;
    s.Step(true, false, false, false, 0);   ok = ok && s.Follows == 1 && s.WanderReset;
    s.Step(false, true, false, false, 0);   ok = ok && s.Follows == 2 && s.AtNoEntry;
    s.Step(false, false, false, false, 0);  ok = ok && s.Follows == 2;
    s.Step(false, false, true, false, 0);   ok = ok && s.Follows == 3 && !s.AtNoEntry;
    s.Step(false, false, false, false, "EXPRESSION_FOLLOW"); ok = ok && s.ContactMade && s.Following && s.Extractions == 1;
    s.Step(false, false, false, false, "EXPRESSION_FOLLOW"); ok = ok && s.Extractions == 1;
    s.Step(false, false, false, false, "EXPRESSION_WAIT");   ok = ok && !s.Following;
    s.Step(false, false, false, true, 0);   ok = ok && s.Outros == 1 && s.TanyaOutros == 1;
    s.Step(false, false, false, true, 0);   ok = ok && s.Outros == 1;
    if (!ok) { std::printf("00f0d630_TEST FAIL\n"); return 1; }
    std::printf("00f0d630_TEST PASS\n");
    return 0;
}