// Behaviour test for CAIStateGroup_GoToPub::ThankBarmaid() @ 0x00949380: a copy of the control flow over a small model
// (speech retry loop, tankard alive test, look-at, animation, barmaid clear, !IsStopping()).
#include <cstdio>

struct Model {
    int speech[8]; int speechN; int speechCalls; int doNothingRet; int doNothingCalls;
    bool tankardSet, tankardDead; int lookRet; int performRet; bool stopping;
    bool barmaidCleared; int lookCalls; int performCalls; unsigned long phraseSeen;
};
static Model m;
class CTCAIScratchpad { public: unsigned long ThankingPhrase; unsigned long GetThankingPhrase() const { return ThankingPhrase; } };
static int PlayAISpeech(unsigned long phrase) { m.phraseSeen = phrase; int r = m.speech[m.speechCalls < m.speechN ? m.speechCalls : m.speechN - 1]; m.speechCalls++; return r; }
static int AIAction_DoNothing() { m.doNothingCalls++; return m.doNothingRet; }

static bool ThankBarmaid(const CTCAIScratchpad* scratchpad)
{
    for (;;) {
        int result = PlayAISpeech(scratchpad->GetThankingPhrase());
        if (result == 2) {
            if (AIAction_DoNothing() == 4)
                continue;
            return false;
        }
        if (result != 0)
            return false;
        break;
    }
    if (!(m.tankardSet && !m.tankardDead))
        return false;
    m.lookCalls++;
    if (m.lookRet == 1)
        return false;
    bool failed;
    {
        m.performCalls++;
        failed = m.performRet == 1;
    }
    if (failed)
        return false;
    m.barmaidCleared = true;
    return m.stopping ? false : true;
}

static void reset() { Model z = Model(); m = z; m.speechN = 1; m.doNothingRet = 4; m.tankardSet = true; m.lookRet = 0; m.performRet = 0; }

int main()
{
    CTCAIScratchpad sp; sp.ThankingPhrase = 0x1234;
    int fail = 0;
    reset(); m.speech[0] = 2; m.speech[1] = 2; m.speech[2] = 0; m.speechN = 3;
    if (!ThankBarmaid(&sp) || m.speechCalls != 3 || m.doNothingCalls != 2 || !m.barmaidCleared || m.phraseSeen != 0x1234) fail |= 1;
    reset(); m.speech[0] = 2; m.doNothingRet = 3;
    if (ThankBarmaid(&sp) || m.lookCalls != 0) fail |= 2;
    reset(); m.speech[0] = 1;
    if (ThankBarmaid(&sp) || m.doNothingCalls != 0) fail |= 4;
    reset(); m.tankardDead = true;
    if (ThankBarmaid(&sp) || m.lookCalls != 0) fail |= 8;
    reset(); m.tankardSet = false;
    if (ThankBarmaid(&sp)) fail |= 16;
    reset(); m.lookRet = 1;
    if (ThankBarmaid(&sp) || m.performCalls != 0) fail |= 32;
    reset(); m.performRet = 1;
    if (ThankBarmaid(&sp) || m.barmaidCleared) fail |= 64;
    reset(); m.stopping = true;
    if (ThankBarmaid(&sp) || !m.barmaidCleared) fail |= 128;
    if (fail) { std::printf("00949380_TEST FAIL\n"); return 1; }
    std::printf("00949380_TEST PASS\n");
    return 0;
}