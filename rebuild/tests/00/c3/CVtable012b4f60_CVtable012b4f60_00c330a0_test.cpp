#include <cstdio>
// Behaviour model of 0x00C330A0: the second argument lands in the first member, the first in the second.
struct CVtable_012b4f60 {
    virtual long Kind() const { return 3; }
    void* m_pSecond; void* m_pFirst;
    CVtable_012b4f60(void* first, void* second) : m_pSecond(second), m_pFirst(first) {}
};
int main()
{
    int a = 1, b = 2;
    CVtable_012b4f60 o(&a, &b);
    if (o.m_pSecond != &b || o.m_pFirst != &a || o.Kind() != 3) { std::printf("00c330a0_TEST FAIL\n"); return 1; }
    std::printf("00c330a0_TEST PASS\n");
    return 0;
}