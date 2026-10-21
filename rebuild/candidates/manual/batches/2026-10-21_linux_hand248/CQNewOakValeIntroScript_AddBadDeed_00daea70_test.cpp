// Behaviour test for 00daea70 (CQ_NewOakValeIntroScript::AddBadDeed): every call counts the deed and takes away the
// per-deed morality; the first bad deed (with no good deeds) shows the first-deed message and logs the map tutorial;
// later deeds show the bad-deed message only once per kind; the deed's flag is always set unless terminating.
#include <cstdio>
#include <cstring>

struct Model {
    long Good, Bad; bool Which[8]; float morality; int msgFirst, msgOther, logbook, frames; bool terminating;
    float perDeed;
    bool Term() { return terminating; }
    bool Clicked() { return frames >= 2; }
    void AddBadDeed(long deed)
    {
        ++Bad; morality += -perDeed;
        if (Bad == 1 && Good == 0) {
            if (Term()) return;
            ++msgFirst; frames = 0;
            while (!Clicked()) { ++frames; if (Term()) return; }
            if (Term()) return;
            ++logbook;
        } else {
            if (Term()) return;
            if (!Which[deed]) {
                if (Term()) return;
                ++msgOther; frames = 0;
                while (!Clicked()) { ++frames; if (Term()) return; }
                if (Term()) return;
            }
        }
        Which[deed] = true;
    }
};
int main()
{
    Model m; std::memset(&m, 0, sizeof m); m.perDeed = 2.5f;
    m.AddBadDeed(3);
    int ok = m.Bad == 1 && m.msgFirst == 1 && m.logbook == 1 && m.Which[3] && m.morality == -2.5f;
    m.AddBadDeed(3);
    if (m.msgOther != 0 || m.Bad != 2) ok = 0;
    m.AddBadDeed(5);
    if (m.msgOther != 1 || !m.Which[5] || m.morality != -7.5f) ok = 0;
    m.terminating = true; m.AddBadDeed(6);
    if (m.Which[6] || m.Bad != 4) ok = 0;
    if (!ok) { std::printf("00daea70_TEST FAIL\n"); return 1; }
    std::printf("00daea70_TEST PASS\n");
    return 0;
}
