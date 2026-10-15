// Behaviour test for CAutoPickupBar::AddObject (0x00647511): an existing def is counted up with its
// time reset, a new def is appended with count 1, and an idle bar (state 1) switches to showing (2).
#include <cstdio>

struct CAutoPickup { long DefIndex, Count; float TimeRemaining; };
struct SNode { SNode* next; SNode* prev; CAutoPickup value; };
struct CAutoPickupBar {
    float displayTime; long m_state; SNode head; SNode pool[4]; int used;
    void push_back(const CAutoPickup& v)
    {
        SNode* n = &pool[used++]; n->value = v;
        n->next = &head; n->prev = head.prev; head.prev->next = n; head.prev = n;
    }
    void AddObject(long defIndex)
    {
        SNode* node;
        for (node = head.next; node != &head; node = node->next) {
            if (node->value.DefIndex == defIndex) {
                node->value.TimeRemaining = displayTime;
                ++node->value.Count;
                goto wake;
            }
        }
        {
            CAutoPickup p; p.TimeRemaining = displayTime; p.DefIndex = defIndex; p.Count = 1;
            push_back(p);
        }
    wake:
        if (m_state == 1) m_state = 2;
    }
};

int main()
{
    CAutoPickupBar b; b.displayTime = 3.0f; b.m_state = 1; b.used = 0; b.head.next = b.head.prev = &b.head;
    b.AddObject(10);
    if (b.used != 1 || b.head.next->value.DefIndex != 10 || b.head.next->value.Count != 1 || b.m_state != 2) { std::printf("00647511_TEST FAIL\n"); return 1; }
    b.head.next->value.TimeRemaining = 0.5f; b.m_state = 3;
    b.AddObject(10);
    if (b.used != 1 || b.head.next->value.Count != 2 || b.head.next->value.TimeRemaining != 3.0f || b.m_state != 3) { std::printf("00647511_TEST FAIL\n"); return 1; }
    b.AddObject(11);
    if (b.used != 2 || b.head.prev->value.DefIndex != 11) { std::printf("00647511_TEST FAIL\n"); return 1; }
    std::printf("00647511_TEST PASS\n");
    return 0;
}