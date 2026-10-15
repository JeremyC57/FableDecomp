// Behaviour test for CV_ChapelOfEvilScript::WatchForQuestInfoDisplay (0x00E69F90): the display-state condition
// (DisplayQuestInfo != (InsideChapel && !AttackedByHero)) and the pump-until-terminating loop.
#include <cstdio>
struct CScript;
struct CInterface { CScript* s; int frames; int terminateAt; void NewScriptFrame(); };
struct CScript {
    CInterface* GSI; bool DisplayQuestInfo; bool InsideChapel; bool AttackedByHero; bool terminating; int checks;
    bool IsActiveThreadTerminating() { ++checks; return terminating; }
    void WatchForQuestInfoDisplay()
    {
        while (!IsActiveThreadTerminating()) {
            if (DisplayQuestInfo ? (AttackedByHero || !InsideChapel) : (InsideChapel && !AttackedByHero)) {
                if (IsActiveThreadTerminating())
                    return;
            }
            GSI->NewScriptFrame();
        }
    }
};
void CInterface::NewScriptFrame() { ++frames; if (frames == terminateAt) s->terminating = true; }
static int Checks(bool display, bool inside, bool attacked)
{
    CScript s; CInterface i; i.s = &s; i.frames = 0; i.terminateAt = 3;
    s.GSI = &i; s.DisplayQuestInfo = display; s.InsideChapel = inside; s.AttackedByHero = attacked; s.terminating = false; s.checks = 0;
    s.WatchForQuestInfoDisplay();
    if (i.frames != 3) return -1;
    return s.checks;
}
int main()
{
    // 4 checks when the condition never holds (3 loop tests + final), 7 when it holds every frame
    for (int m = 0; m < 8; ++m) {
        bool d = (m & 1) != 0, in = (m & 2) != 0, at = (m & 4) != 0;
        bool cond = d != (in && !at);
        if (Checks(d, in, at) != (cond ? 7 : 4)) { std::printf("00e69f90_TEST FAIL\n"); return 1; }
    }
    std::printf("00e69f90_TEST PASS\n");
    return 0;
}