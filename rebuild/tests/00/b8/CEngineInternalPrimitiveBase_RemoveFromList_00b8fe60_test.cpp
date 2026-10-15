#include <cstdio>
struct CCell { long NoPrimitives; };
struct CPrim {
    CCell* GridCell;
    unsigned char Enable : 1; unsigned char NonPersistent : 1;
    CPrim* NextPrimitive; CPrim** RefPrimitive; CPrim* NextLayerMask; CPrim** RefLayerMask;
    CPrim() : GridCell(0), NextPrimitive(0), RefPrimitive(0), NextLayerMask(0), RefLayerMask(0) { Enable = 1; NonPersistent = 0; }
    void RemoveFromList()
    {
        if (!NonPersistent && GridCell) { --GridCell->NoPrimitives; GridCell = 0; }
        if (RefPrimitive) *RefPrimitive = NextPrimitive;
        if (NextPrimitive) NextPrimitive->RefPrimitive = RefPrimitive;
        if (RefLayerMask) {
            *RefLayerMask = NextPrimitive;
            if (NextPrimitive) {
                NextPrimitive->RefLayerMask = RefLayerMask;
                if (NextLayerMask && NextLayerMask != NextPrimitive) {
                    NextPrimitive->NextLayerMask = NextLayerMask;
                    NextLayerMask->RefLayerMask = &NextPrimitive->NextLayerMask;
                }
            }
        }
        NextPrimitive = 0; RefPrimitive = 0; NextLayerMask = 0; RefLayerMask = 0;
    }
};
int main()
{
    // head -> a(mask1 group head) -> b(mask1) -> c(mask2 group head)
    CPrim a, b, c; CPrim* head = &a; CCell cell; cell.NoPrimitives = 3;
    a.RefPrimitive = &head; a.RefLayerMask = &head; a.NextPrimitive = &b; a.NextLayerMask = &c; a.GridCell = &cell;
    b.RefPrimitive = &a.NextPrimitive; b.NextPrimitive = &c;
    c.RefPrimitive = &b.NextPrimitive; c.RefLayerMask = &a.NextLayerMask;
    a.RemoveFromList();
    if (head != &b || b.RefPrimitive != &head || b.RefLayerMask != &head || b.NextLayerMask != &c
        || c.RefLayerMask != &b.NextLayerMask || cell.NoPrimitives != 2 || a.GridCell != 0 || a.NextPrimitive != 0 || a.RefLayerMask != 0) {
        std::printf("00b8fe60_TEST FAIL\n"); return 1;
    }
    c.NonPersistent = 1; c.GridCell = &cell;
    c.RemoveFromList();                          // tail, group head with no successor; non-persistent keeps its cell
    if (b.NextPrimitive != 0 || b.NextLayerMask != 0 || c.GridCell != &cell || cell.NoPrimitives != 2) { std::printf("00b8fe60_TEST FAIL\n"); return 1; }
    std::printf("00b8fe60_TEST PASS\n");
    return 0;
}