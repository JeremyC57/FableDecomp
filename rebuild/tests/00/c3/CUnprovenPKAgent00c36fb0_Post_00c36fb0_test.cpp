// behaviour test for 0x00C36FB0 CUnprovenPKAgent_00c36fb0::Post (model: the receiver sees a message carrying the argument)
#include <cstdio>
static void* g_got = 0;
struct Msg { void* m_first; Msg(void* f) : m_first(f) { } };
struct Receiver { virtual void Receive(Msg* m) { g_got = m->m_first; } virtual ~Receiver() { } };
struct Owner { Receiver m_receiver; void Post(void* a) { Msg message(a); m_receiver.Receive(&message); } };
int main()
{
    int x = 0; Owner o;
    o.Post(&x);
    int ok = g_got == &x;
    if (!ok) { std::printf("00c36fb0_TEST FAIL\n"); return 1; }
    std::printf("00c36fb0_TEST PASS\n");
    return 0;
}