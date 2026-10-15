#include <cstdio>
struct CPrim {
    unsigned int RenderLayerMask;
    CPrim* NextPrimitive; CPrim** RefPrimitive; CPrim* NextLayerMask; CPrim** RefLayerMask;
    CPrim(unsigned int m) : RenderLayerMask(m), NextPrimitive(0), RefPrimitive(0), NextLayerMask(0), RefLayerMask(0) { }
    void AddToList(CPrim** list)
    {
        unsigned int mask = RenderLayerMask;
        for (CPrim* p = *list; p; p = p->NextLayerMask) {
            if (p->RenderLayerMask == mask) {
                NextPrimitive = p->NextPrimitive;
                RefPrimitive = &p->NextPrimitive;
                p->NextPrimitive = this;
                if (NextPrimitive)
                    NextPrimitive->RefPrimitive = &NextPrimitive;
                NextLayerMask = 0;
                RefLayerMask = 0;
                return;
            }
        }
        CPrim* first = *list;
        NextPrimitive = first;
        NextLayerMask = first;
        if (first) {
            first->RefPrimitive = &NextPrimitive;
            first->RefLayerMask = &NextLayerMask;
        }
        RefPrimitive = list;
        RefLayerMask = list;
        *list = this;
    }
};
int main()
{
    CPrim* head = 0;
    CPrim a(1), b(2), c(1), d(1);
    a.AddToList(&head);                         // [a]
    if (head != &a || a.RefPrimitive != &head || a.NextPrimitive != 0) { std::printf("00b8fdf0_TEST FAIL\n"); return 1; }
    b.AddToList(&head);                         // new group at head: b -> a
    if (head != &b || b.NextPrimitive != &a || b.NextLayerMask != &a || a.RefPrimitive != &b.NextPrimitive || a.RefLayerMask != &b.NextLayerMask) { std::printf("00b8fdf0_TEST FAIL\n"); return 1; }
    c.AddToList(&head);                         // same mask as a: inserted after a
    if (a.NextPrimitive != &c || c.RefPrimitive != &a.NextPrimitive || c.NextLayerMask != 0 || c.RefLayerMask != 0) { std::printf("00b8fdf0_TEST FAIL\n"); return 1; }
    d.AddToList(&head);                         // after a, before c
    if (a.NextPrimitive != &d || d.NextPrimitive != &c || c.RefPrimitive != &d.NextPrimitive) { std::printf("00b8fdf0_TEST FAIL\n"); return 1; }
    std::printf("00b8fdf0_TEST PASS\n");
    return 0;
}
