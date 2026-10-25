// Behaviour test for 0x006984A0: CGameCameraManager::RemoveCameraPoint (find in the list, flag when first, unlink + free)
#include <cstdio>
#include <cstdlib>

struct Thing { int id; };
struct Node { Node* next; Node* prev; Thing* item; };

struct Manager {
    bool NewAutoMode;
    Node* header;
    void push_back(Thing* t)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->item = t; n->next = header; n->prev = header->prev;
        header->prev->next = n; header->prev = n;
    }
    void RemoveCameraPoint(Thing& thing)
    {
        Node* it = header->next;
        while (it != header && !(it->item == &thing)) it = it->next;
        if (it != header) {
            if (it == header->next) NewAutoMode = true;
            Node* nx = it->next; Node* pv = it->prev;
            pv->next = nx; nx->prev = pv;
            std::free(it);
        }
    }
    int size() const { int k = 0; for (Node* p = header->next; p != header; p = p->next) ++k; return k; }
};

int main()
{
    Node hdr; hdr.next = &hdr; hdr.prev = &hdr; hdr.item = 0;
    Manager m; m.NewAutoMode = false; m.header = &hdr;
    Thing a = { 1 }, b = { 2 }, c = { 3 }, d = { 4 };
    m.push_back(&a); m.push_back(&b); m.push_back(&c);
    m.RemoveCameraPoint(b);
    if (m.size() != 2 || m.NewAutoMode || hdr.next->item != &a || hdr.prev->item != &c) { std::printf("006984a0_TEST FAIL\n"); return 1; }
    m.RemoveCameraPoint(d);
    if (m.size() != 2 || m.NewAutoMode) { std::printf("006984a0_TEST FAIL\n"); return 1; }
    m.RemoveCameraPoint(a);
    if (m.size() != 1 || !m.NewAutoMode || hdr.next->item != &c) { std::printf("006984a0_TEST FAIL\n"); return 1; }
    std::printf("006984a0_TEST PASS\n");
    return 0;
}