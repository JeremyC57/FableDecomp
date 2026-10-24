#include <cstdio>
// Behaviour test for NCreatureMode::CModeBlock::OnDeactivate (retail 0x008D6D70): the mode goes inactive, and a live
// creature in state 1 has its animation resumed.
struct CThing {
    unsigned char m_state;
    unsigned char m_flags91;
    int suspendCalls;
    bool lastSuspend;
    void SetSuspendAnimation(bool suspend, long reason) { ++suspendCalls; lastSuspend = suspend; (void)reason; }
};
struct CModeBase {
    CThing* ParentThing;
    bool Active;
    void OnDeactivate() { Active = false; }
};
struct CModeBlock : CModeBase {
    void OnDeactivate()
    {
        CModeBase::OnDeactivate();
        if (ParentThing != 0 && !(ParentThing->m_flags91 & 1) && ParentThing->m_state == 1)
            ParentThing->SetSuspendAnimation(false, 0);
    }
};

int main()
{
    CThing t;
    t.m_state = 1; t.m_flags91 = 0; t.suspendCalls = 0; t.lastSuspend = true;
    CModeBlock m;
    m.ParentThing = &t; m.Active = true;
    m.OnDeactivate();
    int pass = !m.Active && t.suspendCalls == 1 && !t.lastSuspend;
    t.m_state = 2; m.Active = true;
    m.OnDeactivate();
    pass = pass && !m.Active && t.suspendCalls == 1;
    t.m_state = 1; t.m_flags91 = 1;
    m.OnDeactivate();
    pass = pass && t.suspendCalls == 1;
    m.ParentThing = 0; m.Active = true;
    m.OnDeactivate();
    pass = pass && !m.Active;
    if (!pass) { std::printf("008d6d70_TEST FAIL\n"); return 1; }
    std::printf("008d6d70_TEST PASS\n");
    return 0;
}