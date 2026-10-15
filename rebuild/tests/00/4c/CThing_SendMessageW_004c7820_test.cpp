// Behaviour test for CThing::SendMessageW @ 0x004C7820: message reaches each live TC with adds forbidden, then re-permitted.
#include <cstdio>
struct CMessage { int id; };
struct CThing;
struct CTCBase {
    bool ToBeRemoved; int calls; CMessage* last; bool addsAllowedDuringCall; CThing* owner;
    CTCBase(bool removed, CThing* o) : ToBeRemoved(removed), calls(0), last(0), addsAllowedDuringCall(true), owner(o) { }
    virtual void HandleMessage(CMessage* message);
};
struct SComponentPair { long first; CTCBase* second; };
struct CThing {
    SComponentPair* m_componentsFirst;
    SComponentPair* m_componentsLast;
    unsigned char   low : 6;
    unsigned char   AddingTCsPermitted : 1;
    void SendMessageW(CMessage* message)
    {
        AddingTCsPermitted = false;
        for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
            CTCBase* tc = it->second;
            if (!tc->ToBeRemoved)
                tc->HandleMessage(message);
        }
        AddingTCsPermitted = true;
    }
};
void CTCBase::HandleMessage(CMessage* message) { ++calls; last = message; addsAllowedDuringCall = owner->AddingTCsPermitted != 0; }
int main()
{
    CThing t; t.low = 0x15; t.AddingTCsPermitted = 0;
    CTCBase a(false, &t), b(true, &t), c(false, &t);
    SComponentPair v[3] = { { 1, &a }, { 2, &b }, { 3, &c } };
    t.m_componentsFirst = v; t.m_componentsLast = v + 3;
    CMessage m; m.id = 3;
    t.SendMessageW(&m);
    if (a.calls != 1 || a.last != &m || a.addsAllowedDuringCall || b.calls != 0 || c.calls != 1 ||
        !t.AddingTCsPermitted || t.low != 0x15) {
        std::printf("004c7820_TEST FAIL\n"); return 1;
    }
    std::printf("004c7820_TEST PASS\n");
    return 0;
}