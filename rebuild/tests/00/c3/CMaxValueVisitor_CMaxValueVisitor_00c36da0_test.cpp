// Behaviour test for 00c36da0: CMaxValueVisitor constructor -- maximum starts at 0.0f, Visit keeps the largest value
#include <cstdio>
struct IValueVisitor { virtual void Visit(void* item) = 0; virtual ~IValueVisitor() {} };
struct CMaxValueVisitor : IValueVisitor {
    float m_max;
    CMaxValueVisitor() : m_max(0) {}
    virtual void Visit(void* item) { float v = *(float*)item; if (v > m_max) m_max = v; }
};
int main()
{
    CMaxValueVisitor v; bool ok = v.m_max == 0.0f; IValueVisitor* p = &v;
    float a = 2.5f, b = -1.0f, c = 1.0f; p->Visit(&a); p->Visit(&b); p->Visit(&c);
    ok = ok && v.m_max > 2.49f && v.m_max < 2.51f && sizeof(v) == 8;
    if (!ok) { std::printf("00c36da0_TEST FAIL\n"); return 1; }
    std::printf("00c36da0_TEST PASS\n");
    return 0;
}
