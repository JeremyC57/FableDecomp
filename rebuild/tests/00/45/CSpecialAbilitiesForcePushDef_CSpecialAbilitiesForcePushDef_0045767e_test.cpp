#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <new>
static int g_baseCalls;
class CParentDefClassBase { public: virtual void v00() {} unsigned char _body[0x74]; CParentDefClassBase() { ++g_baseCalls; } };
template <class T> struct CRetailVector { T* first; T* last; T* end; CRetailVector() : first(0), last(0), end(0) { } };
class CSpecialAbilitiesForcePushDef : public CParentDefClassBase { public:
    CRetailVector<long> m_values0; unsigned char _pad[0x08]; CRetailVector<long> m_values1; CRetailVector<long> m_values2;
    virtual void v00() {}
    CSpecialAbilitiesForcePushDef();
};
CSpecialAbilitiesForcePushDef::CSpecialAbilitiesForcePushDef() : CParentDefClassBase(), m_values0(), m_values1(), m_values2() { }
int main()
{
    unsigned char raw[sizeof(CSpecialAbilitiesForcePushDef)]; memset(raw, 0xEE, sizeof(raw));
    CSpecialAbilitiesForcePushDef* d = new (raw) CSpecialAbilitiesForcePushDef();
    bool ok = g_baseCalls == 1 && d->m_values0.first == 0 && d->m_values0.end == 0 && d->m_values1.first == 0 && d->m_values1.last == 0 && d->m_values2.end == 0 && d->_pad[0] == 0xEE
        && offsetof(CSpecialAbilitiesForcePushDef, m_values0) == 0x78 && offsetof(CSpecialAbilitiesForcePushDef, m_values1) == 0x8c && offsetof(CSpecialAbilitiesForcePushDef, m_values2) == 0x98;
    if (!ok) { printf("0045767e_TEST FAIL\n"); return 1; }
    printf("0045767e_TEST PASS\n"); return 0;
}