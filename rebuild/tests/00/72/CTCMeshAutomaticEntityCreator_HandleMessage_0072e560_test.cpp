// Behaviour test for CTCMeshAutomaticEntityCreator::HandleMessage @ 0x0072E560: only type 5 reaches CatchMessage.
#include <cstdio>
struct CTCMessage {
    long m_type;
    virtual long GetType() const { return m_type; }
    virtual ~CTCMessage() {}
};
struct CTCMeshAutomaticEntityCreator {
    int m_caught;
    CTCMessage* m_last;
    void CatchMessage(CTCMessage* msg) { ++m_caught; m_last = msg; }
    void HandleMessage(CTCMessage* msg) { if (msg->GetType() == 5) CatchMessage(msg); }
};
int main()
{
    CTCMeshAutomaticEntityCreator c; c.m_caught = 0; c.m_last = 0;
    CTCMessage a; a.m_type = 4;
    CTCMessage b; b.m_type = 5;
    c.HandleMessage(&a);
    if (c.m_caught != 0) { std::printf("0072e560_TEST FAIL\n"); return 1; }
    c.HandleMessage(&b);
    if (c.m_caught != 1 || c.m_last != &b) { std::printf("0072e560_TEST FAIL\n"); return 1; }
    std::printf("0072e560_TEST PASS\n");
    return 0;
}