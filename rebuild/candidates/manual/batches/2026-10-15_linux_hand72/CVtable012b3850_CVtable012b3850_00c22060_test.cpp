#include <cstdio>
// Behaviour model of 0x00C22060: root polymorphic class keeping both arguments in order.
struct CVtable_012b3850 {
    virtual long Kind() const { return 2; }
    void* m_pFirst; void* m_pSecond;
    CVtable_012b3850(void* first, void* second) : m_pFirst(first), m_pSecond(second) {}
};
int main()
{
    int a = 1, b = 2;
    CVtable_012b3850 o(&a, &b);
    if (o.m_pFirst != &a || o.m_pSecond != &b || o.Kind() != 2) { std::printf("00c22060_TEST FAIL\n"); return 1; }
    std::printf("00c22060_TEST PASS\n");
    return 0;
}
