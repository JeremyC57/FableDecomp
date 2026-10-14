#include <cstdio>
struct CLifetimedObject { unsigned char _pad[0x1e]; unsigned short m_lifetime; void DecrementLifetimer() { --m_lifetime; } };
struct CLifetimeRef { CLifetimedObject* m_pObject; ~CLifetimeRef() { m_pObject->DecrementLifetimer(); m_pObject = 0; } };
int main()
{
    CLifetimedObject o; o.m_lifetime = 3;
    { CLifetimeRef r; r.m_pObject = &o; }
    if (o.m_lifetime != 2) { std::printf("00c26060_TEST FAIL\n"); return 1; }
    std::printf("00c26060_TEST PASS\n"); return 0;
}
