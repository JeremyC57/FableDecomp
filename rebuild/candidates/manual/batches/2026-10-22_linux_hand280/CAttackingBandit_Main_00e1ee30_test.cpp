// Behaviour test for CAttackingBandit::Main (0x00E1EE30): with the escorted trader alive, Me turns on the hero and the trader and follows the trader; the first time it waits for the hero within 10 units and plays the ambush laugh with the parent's laugh index appended; frames run while the trader lives.
#include <cstdio>
#include <cstring>
struct CSim { int frame; int traderDiesAt; int heroCloseAt; int terminateAt; int enemies; int following; char sound[64]; };
struct CBandit {
    CSim* S; bool Screamed; int LaughIndex; bool TraderExists;
    bool Terminating() const { return S->frame >= S->terminateAt; }
    bool TraderAlive() const { return TraderExists && S->frame < S->traderDiesAt; }
    void Main()
    {
        ++S->frame;
        if (Terminating()) return;
        if (TraderAlive()) {
            S->enemies += 2;
            S->following = 1;
            if (!Screamed) {
                if (Terminating()) return;
                while (S->frame < S->heroCloseAt) { ++S->frame; if (Terminating()) return; }
                if (Terminating()) return;
                std::strcpy(S->sound, "SND_MAN_05_NONSPEECH_AMBUSHLAUGH_0");
                char digits[8]; std::sprintf(digits, "%d", LaughIndex);
                std::strcat(S->sound, digits);
                Screamed = true;
            }
            while (TraderAlive()) { ++S->frame; if (Terminating()) return; }
        }
    }
};
int main()
{
    CSim s; std::memset(&s, 0, sizeof(s)); s.traderDiesAt = 9; s.heroCloseAt = 4; s.terminateAt = 50;
    CBandit b; b.S = &s; b.Screamed = false; b.LaughIndex = 3; b.TraderExists = true;
    b.Main();
    bool ok = s.enemies == 2 && s.following == 1 && b.Screamed && std::strcmp(s.sound, "SND_MAN_05_NONSPEECH_AMBUSHLAUGH_03") == 0
        && s.frame == 9;
    CSim s2; std::memset(&s2, 0, sizeof(s2)); s2.terminateAt = 50;
    b.S = &s2; b.Screamed = false; b.TraderExists = false;
    b.Main();
    ok = ok && s2.enemies == 0 && !b.Screamed && s2.sound[0] == 0;
    if (!ok) { std::printf("00e1ee30_TEST FAIL\n"); return 1; }
    std::printf("00e1ee30_TEST PASS\n");
    return 0;
}
