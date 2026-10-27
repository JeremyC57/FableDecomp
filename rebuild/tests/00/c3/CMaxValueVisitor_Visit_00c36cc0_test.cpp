// Test for 0x00C36CC0 CMaxValueVisitor::Visit: the maximum rises only for a larger item value.
#include <cstdio>
struct CItem { float value; float M() { return value; } };
struct CMaxValueVisitor { float m_max; void Visit(void* item) { CItem* s = (CItem*)item; if (s->M() > m_max) m_max = s->M(); } };
int main()
{
    CItem a = {2.5f}, b = {1.0f}, c = {2.5f}, d = {4.0f};
    CMaxValueVisitor v; v.m_max = 0.0f;
    v.Visit(&a); v.Visit(&b); v.Visit(&c);
    bool ok = v.m_max == 2.5f; v.Visit(&d); ok = ok && v.m_max == 4.0f;
    if (!ok) { std::printf("00c36cc0_TEST FAIL\n"); return 1; }
    std::printf("00c36cc0_TEST PASS\n");
    return 0;
}