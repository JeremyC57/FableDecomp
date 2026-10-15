// Behaviour test for CThing::InitTC @ 0x004C73E0: runs the staged initialisers up to the thing's stage, skips removed TCs.
#include <cstdio>
struct CTCBase {
    int  calls[3];
    bool ToBeRemoved;
    CTCBase() : ToBeRemoved(false) { calls[0] = calls[1] = calls[2] = 0; }
    virtual void InitialiseStage1() { ++calls[0]; }
    virtual void InitialiseStage2() { ++calls[1]; }
    virtual void InitialiseStage3() { ++calls[2]; }
};
struct CThing {
    unsigned char InitialisationStage : 3;
    void InitTC(CTCBase* tc)
    {
        if (!tc->ToBeRemoved) {
            unsigned char stage = InitialisationStage;
            if (stage == 1) {
                tc->InitialiseStage1();
            } else if (stage == 2) {
                tc->InitialiseStage1();
                tc->InitialiseStage2();
            } else if (stage == 3) {
                tc->InitialiseStage1();
                tc->InitialiseStage2();
                tc->InitialiseStage3();
            }
        }
    }
};
static bool Check(int stage, bool removed, int a, int b, int c)
{
    CThing t; t.InitialisationStage = (unsigned char)stage;
    CTCBase tc; tc.ToBeRemoved = removed;
    t.InitTC(&tc);
    return tc.calls[0] == a && tc.calls[1] == b && tc.calls[2] == c;
}
int main()
{
    if (!Check(0, false, 0, 0, 0) || !Check(1, false, 1, 0, 0) || !Check(2, false, 1, 1, 0) ||
        !Check(3, false, 1, 1, 1) || !Check(4, false, 0, 0, 0) || !Check(3, true, 0, 0, 0)) {
        std::printf("004c73e0_TEST FAIL\n"); return 1;
    }
    std::printf("004c73e0_TEST PASS\n");
    return 0;
}