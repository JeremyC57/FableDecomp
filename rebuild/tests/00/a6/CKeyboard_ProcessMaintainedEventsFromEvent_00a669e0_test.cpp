#include <cstdio>
#include <stdlib.h>
#include <new>
class CInputEvent { public:
    long Kind; long Key; float StartTime; float CurrentTime;
    long GetType() const { return Kind; }
    long GetKey() const { return Key; }
    void SetAsKeyboardKeyHeld(long key, double startTime, double currentTime)
    { Kind = 2; Key = key; StartTime = (float)startTime; CurrentTime = (float)currentTime; }
};
static int g_dtors = 0;
class CMaintainedEvent { public:
    virtual ~CMaintainedEvent() { ++g_dtors; }
    CInputEvent Event;
};
struct CMaintainedEventList {
    struct Node { Node* next; Node* prev; CMaintainedEvent value; };
    struct iterator {
        Node* ptr;
        iterator() { }
        iterator(Node* p) : ptr(p) { }
        iterator(const iterator& o) : ptr(o.ptr) { }
    };
    Node* head;
    iterator end() { return iterator(head); }
    iterator insert(iterator where)
    {
        Node* n = (Node*)malloc(sizeof(Node));
        new (&n->value) CMaintainedEvent();
        n->value.Event.Kind = 0; n->value.Event.Key = 0;
        n->next = where.ptr; n->prev = where.ptr->prev; where.ptr->prev->next = n; where.ptr->prev = n;
        return iterator(n);
    }
    Node* last() { return head->prev; }
};
class CKeyboard { public:
    CMaintainedEventList MaintainedEventsList;
    void ProcessMaintainedEventsFromEvent(const CInputEvent& event);
};
void CKeyboard::ProcessMaintainedEventsFromEvent(const CInputEvent& event)
{
    if (event.GetType() == 1) {
        MaintainedEventsList.insert(MaintainedEventsList.end());
        MaintainedEventsList.last()->value.Event.SetAsKeyboardKeyHeld(event.GetKey(), event.StartTime, event.StartTime);
        return;
    }
    if (event.GetType() == 3) {
        CMaintainedEventList::Node* node = MaintainedEventsList.head->next;
        while (node != MaintainedEventsList.head) {
            if (node->value.Event.GetType() == 2) {
                if (node->value.Event.GetKey() == event.GetKey()) {
                    CMaintainedEventList::Node* next = node->next;
                    CMaintainedEventList::Node* prev = node->prev;
                    prev->next = next;
                    next->prev = prev;
                    node->value.~CMaintainedEvent();
                    free(node);
                    node = next;
                } else {
                    node = node->next;
                }
            }
        }
    }
}
static int count(CKeyboard& k) { int n = 0; for (CMaintainedEventList::Node* p = k.MaintainedEventsList.head->next; p != k.MaintainedEventsList.head; p = p->next) ++n; return n; }
static int fail() { std::printf("00a669e0_TEST FAIL\n"); return 1; }
int main()
{
    CMaintainedEventList::Node* head = (CMaintainedEventList::Node*)malloc(sizeof(CMaintainedEventList::Node));
    head->next = head; head->prev = head;
    CKeyboard k; k.MaintainedEventsList.head = head;
    CInputEvent e; e.Kind = 1; e.Key = 30; e.StartTime = 2.5f; e.CurrentTime = 0;
    k.ProcessMaintainedEventsFromEvent(e);
    e.Key = 31; e.StartTime = 3.0f; k.ProcessMaintainedEventsFromEvent(e);
    e.Key = 30; k.ProcessMaintainedEventsFromEvent(e);
    if (count(k) != 3) return fail();
    CMaintainedEventList::Node* first = head->next;
    if (first->value.Event.Kind != 2 || first->value.Event.Key != 30 || first->value.Event.StartTime != 2.5f || first->value.Event.CurrentTime != 2.5f) return fail();
    e.Kind = 5; k.ProcessMaintainedEventsFromEvent(e);
    if (count(k) != 3) return fail();
    e.Kind = 3; e.Key = 30; k.ProcessMaintainedEventsFromEvent(e);
    if (count(k) != 1 || g_dtors != 2 || head->next->value.Event.Key != 31 || head->prev != head->next) return fail();
    e.Key = 31; k.ProcessMaintainedEventsFromEvent(e);
    if (count(k) != 0 || g_dtors != 3 || head->prev != head) return fail();
    std::printf("00a669e0_TEST PASS\n");
    return 0;
}