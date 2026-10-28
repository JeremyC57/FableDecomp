extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x00811380 CTCEntityEvents::HasEventOfTypeCreatedAfterFrame: the circular event list holds an event of
// the type whose WorldFrame is strictly after the frame.
struct CEventBase { long type; long WorldFrame; };
struct Node { Node* next; Node* prev; CEventBase* ev; };
class CTCEntityEvents { public:
    Node* head;
    bool HasEventOfTypeCreatedAfterFrame(long type, long frame) const
    {
        Node* it = head->next;
        while (it != head && !(it->ev->type == type && frame < it->ev->WorldFrame)) it = it->next;
        return it != head;
    }
};
int main()
{
    CEventBase e1 = { 0xe, 10 }, e2 = { 3, 50 }, e3 = { 0xe, 30 };
    Node h, n1, n2, n3;
    h.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &h; n1.ev = &e1; n2.ev = &e2; n3.ev = &e3;
    CTCEntityEvents t; t.head = &h;
    CHECK(t.HasEventOfTypeCreatedAfterFrame(0xe, 20));
    CHECK(!t.HasEventOfTypeCreatedAfterFrame(0xe, 30));
    CHECK(t.HasEventOfTypeCreatedAfterFrame(3, 49));
    CHECK(!t.HasEventOfTypeCreatedAfterFrame(4, 0));
    h.next = &h; CHECK(!t.HasEventOfTypeCreatedAfterFrame(0xe, 0));
    if (fails == 0) printf("00811380_TEST PASS\n"); else printf("00811380_TEST FAIL\n");
    return fails;
}