// Behaviour test for CVtable_012b1b40::Invoke (0x00C088B0): the wrapper runs slot 0 of the object it wraps, not its own state.
#include <stdio.h>
struct CVtable_012b1b40 {
    CVtable_012b1b40* m_pTarget;
    CVtable_012b1b40() : m_pTarget(0) { }
    virtual void Invoke();
};
void CVtable_012b1b40::Invoke()
{
    m_pTarget->Invoke();
}
struct Leaf : CVtable_012b1b40 {
    int calls;
    Leaf() : calls(0) { }
    virtual void Invoke() { ++calls; }
};
int main(void)
{
    Leaf leaf; CVtable_012b1b40 inner; CVtable_012b1b40 outer;
    inner.m_pTarget = &leaf; outer.m_pTarget = &inner;
    inner.Invoke();
    outer.Invoke();      // two hops
    if (leaf.calls != 2) { printf("00c088b0_TEST FAIL\n"); return 1; }
    printf("00c088b0_TEST PASS\n");
    return 0;
}