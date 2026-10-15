// Behaviour test for 0x004B14F0: a parameter is appended only when its thing UID is not in the list yet; the changed flag is raised.
#include <cstdio>
struct Param { const char* name; const char* value; bool flag; int uid; };
struct Node { Node* next; Node* prev; Param data; };
struct M {
    Node header; bool changed;
    M() : changed(false) { header.next = &header; header.prev = &header; }
    Node* Find(int uid) { Node* end = &header; Node* it = end->next; while (it != end && it->data.uid != uid) it = it->next; return it; }
    void Add(int uid, const char* name, const char* value)
    {
        if (Find(uid) == &header) {
            Param p; p.name = name; p.value = value; p.flag = false; p.uid = uid;
            Node* n = new Node; n->data = p; n->next = &header; n->prev = header.prev; header.prev->next = n; header.prev = n;
            changed = true;
        }
    }
    int Count() { int c = 0; for (Node* n = header.next; n != &header; n = n->next) ++c; return c; }
};
int main()
{
    M m;
    m.Add(5, "a", "b");
    bool ok = m.changed && m.Count() == 1 && m.header.next->data.uid == 5 && !m.header.next->data.flag;
    m.changed = false;
    m.Add(5, "c", "d");
    ok = ok && !m.changed && m.Count() == 1;
    m.Add(6, "c", "d");
    ok = ok && m.changed && m.Count() == 2 && m.header.prev->data.uid == 6;
    while (m.header.next != &m.header) { Node* n = m.header.next; m.header.next = n->next; delete n; }
    if (!ok) { std::printf("004b14f0_TEST FAIL\n"); return 1; }
    std::printf("004b14f0_TEST PASS\n");
    return 0;
}
