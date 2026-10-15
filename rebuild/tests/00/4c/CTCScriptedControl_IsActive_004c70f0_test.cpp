#include <cstdio>
struct SNode { SNode* next; };
struct CTCScriptedControl {
    SNode* m_pTasks;
    unsigned char m_locked;
    bool AreTasksEmpty() { return m_pTasks->next == m_pTasks; }
    long IsActive()
    {
        if (!m_locked && AreTasksEmpty())
            return 0;
        return 1;
    }
};
int main()
{
    SNode head; head.next = &head; SNode n; n.next = &head;
    CTCScriptedControl c; c.m_pTasks = &head; c.m_locked = 0;
    if (c.IsActive() != 0) { std::printf("004c70f0_TEST FAIL\n"); return 1; }
    c.m_locked = 1;
    if (c.IsActive() != 1) { std::printf("004c70f0_TEST FAIL\n"); return 1; }
    c.m_locked = 0; head.next = &n;
    if (c.IsActive() != 1) { std::printf("004c70f0_TEST FAIL\n"); return 1; }
    std::printf("004c70f0_TEST PASS\n");
    return 0;
}