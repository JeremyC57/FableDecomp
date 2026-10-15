// Behaviour test for 0x00904EE0 CScriptGameResourceObjectScriptedThing::IsPerformingScriptTask: host model of
// the thing's interface lookup and the scripted-control component's action list (empty = head->next == head).
#include <cstdio>
struct Node { Node* next; };
struct Control { Node head; Control() { head.next = &head; } bool IsActionListEmpty() { return head.next == &head; } };
struct Entry { long type; Control* component; };
struct Thing {
    unsigned int bits[9]; Entry entries[4]; int count; bool dead;
    Thing() : count(0), dead(false) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(long t, Control* c) { entries[count].type = t; entries[count].component = c; ++count; entries[count].type = 0; entries[count].component = 0; bits[t >> 5] |= 1u << (t & 31); }
    Entry* LowerBound(const long* t) { int i = 0; while (i < count && entries[i].type < *t) ++i; return &entries[i]; }
    template<class T> bool GetInterface(T** out, long type)
    {
        if (bits[type >> 5] & (1u << (type & 31))) {
            long key = type; Entry* it = LowerBound(&key); Entry* last = &entries[count];
            if (it == last || type < it->type) it = last;
            *out = it->component; return true;
        }
        return false;
    }
};
struct IP { Thing* item; Thing* GetPItem() { return item; } };
struct ScriptedThing {
    IP PThing;
    bool IsPerformingScriptTask()
    {
        if (PThing.GetPItem()) {
            if (!PThing.GetPItem()->dead) {
                Control* control = 0;
                PThing.GetPItem()->GetInterface(&control, 0x1f);
                return !control->IsActionListEmpty();
            }
        }
        return false;
    }
};
int main()
{
    Thing t; Control c; t.Add(0x1f, &c);
    ScriptedThing s; s.PThing.item = &t;
    if (s.IsPerformingScriptTask()) { std::printf("00904ee0_TEST FAIL\n"); return 1; }
    Node n; n.next = &c.head; c.head.next = &n;
    if (!s.IsPerformingScriptTask()) { std::printf("00904ee0_TEST FAIL\n"); return 1; }
    t.dead = true;
    if (s.IsPerformingScriptTask()) { std::printf("00904ee0_TEST FAIL\n"); return 1; }
    s.PThing.item = 0;
    if (s.IsPerformingScriptTask()) { std::printf("00904ee0_TEST FAIL\n"); return 1; }
    std::printf("00904ee0_TEST PASS\n");
    return 0;
}
