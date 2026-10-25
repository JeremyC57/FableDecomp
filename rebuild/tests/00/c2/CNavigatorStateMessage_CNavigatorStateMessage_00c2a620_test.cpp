#include <cstdio>
#include <new>
struct CNavigatorStateMessage {
    virtual long Kind() { return 7; }
    CNavigatorStateMessage(unsigned char state) : m_state(state) { }
    unsigned char m_state;
};
int main()
{
    char buf[sizeof(CNavigatorStateMessage)];
    CNavigatorStateMessage* m = new (buf) CNavigatorStateMessage(5);
    if ((void*)m != (void*)buf || m->m_state != 5 || m->Kind() != 7 || sizeof(CNavigatorStateMessage) != 8) { std::printf("00c2a620_TEST FAIL\n"); return 1; }
    std::printf("00c2a620_TEST PASS\n");
    return 0;
}
