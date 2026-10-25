// Behaviour test for 00c1df50: CReplyMailbox constructor -- the queue first, then the mailbox over it with capacity 1
#include <cstdio>
static int g_order = 0, g_queueAt = 0, g_mailboxAt = 0; static long g_cap = -1; static void* g_q = 0;
struct Queue { unsigned char m_body[0x10]; Queue() { g_queueAt = ++g_order; } };
struct Mailbox { unsigned char m_body[0x20]; Mailbox(Queue* q, long c) { g_mailboxAt = ++g_order; g_q = q; g_cap = c; } };
struct CReplyMailbox { Queue m_queue; Mailbox m_mailbox; CReplyMailbox() : m_queue(), m_mailbox(&m_queue, 1) {} };
int main()
{
    CReplyMailbox r;
    if (g_queueAt != 1 || g_mailboxAt != 2 || g_q != &r.m_queue || g_cap != 1 || sizeof(r) != 0x30)
    { std::printf("00c1df50_TEST FAIL\n"); return 1; }
    std::printf("00c1df50_TEST PASS\n");
    return 0;
}
