#include <cstdio>

class C3DAnimationStateAlpha { public: int tag; };

class CTCPerceiveThing { public:
    int aCalls;
    int bCalls;
    C3DAnimationStateAlpha* lastArg;
    int seq;

    void helperA() { aCalls++; seq = seq*10 + 1; }
    void helperB(C3DAnimationStateAlpha* a) { bCalls++; lastArg = a; seq = seq*10 + 2; }
    void AddCombinationBlend(C3DAnimationStateAlpha* a);
};

void CTCPerceiveThing::AddCombinationBlend(C3DAnimationStateAlpha* a)
{
    this->helperA();
    this->helperB(a);
}

int main() {
    CTCPerceiveThing s = {0,0,0,0};
    C3DAnimationStateAlpha alpha = {42};
    s.AddCombinationBlend(&alpha);
    bool ok = (s.aCalls==1) && (s.bCalls==1) && (s.lastArg==&alpha) && (s.seq==12);
    if (ok) printf("00744f90_TEST PASS\n");
    else printf("FAIL a=%d b=%d seq=%d\n", s.aCalls, s.bCalls, s.seq);
    return 0;
}