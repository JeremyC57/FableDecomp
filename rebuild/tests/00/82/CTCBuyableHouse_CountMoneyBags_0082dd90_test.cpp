// behaviour test for 0x0082DD90 (thing: any live contained thing with interface 0x26)
#include <cstdio>
struct CThing;
struct Ptr { CThing* p; CThing* Get() const { return p; } };
struct Node { Node* next; Node* prev; Ptr thing; };
struct List { Node* head; };
struct Entry { long type; void* component; };
struct CThing {
    unsigned int bits[9];
    Entry* first; Entry* last;
    unsigned char flags;
    Entry* LowerBound(const long* t) { Entry* e = first; while (e != last && e->type < *t) ++e; return e; }
    template<class T> bool GetInterface(T** out, long type) {
        if (bits[type >> 5] & (1u << (type & 31))) {
            long k = type; Entry* it = LowerBound(&k);
            if (it == last || type < it->type) it = last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
    unsigned int HasInterface(long t) const { return bits[t >> 5] & (1u << (t & 31)); }
    bool CountMoneyBags() {
        List* list;
        if (GetInterface(&list, 0x21)) {
            long count = 0;
            for (Node* n = list->head->next; n != list->head; n = n->next)
                if (n->thing.Get() && !(n->thing.Get()->flags & 1) && n->thing.Get()->HasInterface(0x26)) ++count;
            return count > 0;
        }
        return false;
    }
};
static CThing mk() { CThing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0; t.first = t.last = 0; t.flags = 0; return t; }
int main()
{
    CThing owner = mk(), a = mk(), b = mk();
    Node head, n1, n2;
    List list; list.head = &head;
    head.next = &n1; n1.next = &n2; n2.next = &head;
    n1.thing.p = &a; n2.thing.p = &b;
    Entry e[2]; e[0].type = 0x21; e[0].component = &list; owner.first = e; owner.last = e + 1;
    bool ok = true;
    if (owner.CountMoneyBags()) ok = false;               // interface bit clear
    owner.bits[1] = 2;
    if (owner.CountMoneyBags()) ok = false;               // no thing has 0x26
    b.bits[1] = 0x40; b.flags = 1;
    if (owner.CountMoneyBags()) ok = false;               // dead
    b.flags = 0;
    if (!owner.CountMoneyBags()) ok = false;
    n2.thing.p = 0;
    if (owner.CountMoneyBags()) ok = false;               // null pointer
    if (!ok) { std::printf("0082dd90_TEST FAIL\n"); return 1; }
    std::printf("0082dd90_TEST PASS\n");
    return 0;
}