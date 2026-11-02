// Behaviour test for 0x00C0F440 CVtable_012b2308::GetFirst_00c0f440: the shell forwards slot 11 to its implementation and returns its result.
#include <stdio.h>
class CUnprovenContainerImplementation_00c0f440 {
public:
    virtual void Slot00() = 0;
    virtual void Slot01() = 0;
    virtual void Slot02() = 0;
    virtual void Slot03() = 0;
    virtual void Slot04() = 0;
    virtual void Slot05() = 0;
    virtual void Slot06() = 0;
    virtual void Slot07() = 0;
    virtual void Slot08() = 0;
    virtual void Slot09() = 0;
    virtual void Slot10() = 0;
    virtual void* GetFirst() = 0;
};
class CVtable_012b2308 {
public:
    CVtable_012b2308(CUnprovenContainerImplementation_00c0f440* implementation) : Implementation(implementation) {}
    virtual void Slot00() = 0;
    virtual void Slot01() = 0;
    virtual void Slot02() = 0;
    virtual void Slot03() = 0;
    virtual ~CVtable_012b2308() {}
    virtual void Slot05() {}
    virtual void Slot06() {}
    virtual void Slot07() {}
    virtual void Slot08() {}
    virtual void Slot09() {}
    virtual void Slot10() {}
    virtual void* GetFirst_00c0f440();
    CUnprovenContainerImplementation_00c0f440* Implementation;
};
void* CVtable_012b2308::GetFirst_00c0f440()
{
    return Implementation->GetFirst();
}
// --- harness ---
static int g_calls = 0;
class FakeTree : public CUnprovenContainerImplementation_00c0f440 {
public:
    void* first;
    FakeTree(void* f) : first(f) {}
    void Slot00() {} void Slot01() {} void Slot02() {} void Slot03() {} void Slot04() {} void Slot05() {}
    void Slot06() {} void Slot07() {} void Slot08() {} void Slot09() {} void Slot10() {}
    void* GetFirst() { ++g_calls; return first; }
};
class Shell : public CVtable_012b2308 {
public:
    Shell(CUnprovenContainerImplementation_00c0f440* p) : CVtable_012b2308(p) {}
    void Slot00() {} void Slot01() {} void Slot02() {} void Slot03() {}
};
int main()
{
    int a = 7, b = 9;
    FakeTree t1(&a), t2(&b), t3(0);
    Shell s1(&t1), s2(&t2), s3(&t3);
    CVtable_012b2308* p = &s1;
    if (p->GetFirst_00c0f440() != &a) { printf("FAIL 1\n"); return 1; }
    p = &s2;
    if (p->GetFirst_00c0f440() != &b) { printf("FAIL 2\n"); return 1; }
    p = &s3;
    if (p->GetFirst_00c0f440() != 0) { printf("FAIL 3\n"); return 1; }
    s1.Implementation = &t2;                       // the shell reads +4 on every call
    if (s1.GetFirst_00c0f440() != &b) { printf("FAIL 4\n"); return 1; }
    if (g_calls != 4) { printf("FAIL calls %d\n", g_calls); return 1; }
    printf("00c0f440_TEST PASS\n");
    return 0;
}