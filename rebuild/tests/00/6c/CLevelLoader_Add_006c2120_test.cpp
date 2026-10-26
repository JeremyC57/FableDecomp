// Behaviour test for 0x006C2120 CLevelLoader::Add(const CCountedPointer<CLevelLoader::CObject>&).
#include <cstdio>
#include <cstdlib>
#include <new>
struct Info { long RefCount; };
class CLevelLoader;
struct CObject {
    const CLevelLoader* PLevelLoader;
    int adds;
    void OnAdd(const CLevelLoader& loader) { PLevelLoader = &loader; ++adds; }
};
struct CP {
    CObject* Data; Info* PInfo;
    CP(const CP& o) : Data(o.Data), PInfo(o.PInfo) { if (PInfo) ++PInfo->RefCount; }
    CP(CObject* d, Info* i) : Data(d), PInfo(i) { }
    CObject* operator->() const { return Data; }
};
struct Node { Node* next; Node* prev; CP value; };
class CLevelLoader { public:
    Node* head;
    CLevelLoader() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    ~CLevelLoader() { Node* n = head->next; while (n != head) { Node* x = n->next; std::free(n); n = x; } std::free(head); }
    void Add(const CP& object) {
        object->OnAdd(*this);
        Node* pos = head;
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        new (&tmp->value) CP(object);
        Node* prev = pos->prev;
        tmp->next = pos; tmp->prev = prev; prev->next = tmp; pos->prev = tmp;
    }
};
int main() {
    int ok = 1;
    CObject a = { 0, 0 }, b = { 0, 0 };
    Info ia = { 1 }, ib = { 1 };
    CP pa(&a, &ia), pb(&b, &ib), pn(&b, 0);
    CLevelLoader loader;
    loader.Add(pa);
    loader.Add(pb);
    loader.Add(pa);
    loader.Add(pn);                              // no info: the count is not touched
    ok &= a.PLevelLoader == &loader && b.PLevelLoader == &loader && a.adds == 2 && b.adds == 2;
    ok &= ia.RefCount == 3 && ib.RefCount == 2;
    CObject* order[4] = { &a, &b, &a, &b };
    int n = 0;
    for (Node* p = loader.head->next; p != loader.head; p = p->next, ++n)
        ok &= n < 4 && p->value.Data == order[n] && p->next->prev == p;
    ok &= n == 4 && loader.head->prev->value.PInfo == 0;
    if (!ok) { std::printf("006c2120_TEST FAIL\n"); return 1; }
    std::printf("006c2120_TEST PASS\n");
    return 0;
}