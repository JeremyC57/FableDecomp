// Behaviour test for 0x00C3F490 ~CVtable_012b5c60: Stop() only when still running, member and base destroyed after it.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
struct CListNode { virtual ~CListNode() { g_log[g_n++] = 3; } };
struct Base : CListNode {
    unsigned int m_state : 31; unsigned int m_stopped : 1;
    Base() : m_state(0), m_stopped(0) { }
    bool IsRunning() const { return !m_stopped; }
    virtual void Stop() { g_log[g_n++] = 9; }
    virtual ~Base() { if (IsRunning()) Stop(); }
};
struct Member { virtual ~Member() { g_log[g_n++] = 2; } };
struct Derived : Base {
    Member m_member;
    virtual void Stop() { g_log[g_n++] = 1; m_stopped = 1; }
    virtual ~Derived() { if (IsRunning()) Stop(); }
};
int main()
{
    { Derived d; }
    bool ok = g_n == 3 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3;
    g_n = 0;
    { Derived d; d.m_stopped = 1; }
    ok = ok && g_n == 2 && g_log[0] == 2 && g_log[1] == 3;
    if (!ok) { std::printf("00c3f490_TEST FAIL\n"); return 1; }
    std::printf("00c3f490_TEST PASS\n");
    return 0;
}