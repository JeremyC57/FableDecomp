// Test for 0x004E0A67 CShipDef::Copy(const CDefClassBase*): through the base pointer, assigns the other def to this one
// (exactly one operator= call, with this and the other object).
#include <stdio.h>
static const void* g_lhs; static const void* g_rhs; static int g_calls;
class CDefClassBase {
public:
    int Base;
    virtual ~CDefClassBase() { }
    virtual void Copy(const CDefClassBase* other) { Base = other->Base; }
};
class CShipDef : public CDefClassBase {
public:
    int Value;
    CShipDef& operator=(const CShipDef& other) { ++g_calls; g_lhs = this; g_rhs = &other; Base = other.Base; Value = other.Value; return *this; }
    virtual void Copy(const CDefClassBase* other);
};
void CShipDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CShipDef*>(other);
}
int main()
{
    CShipDef a, b; a.Base = 1; a.Value = 2; b.Base = 30; b.Value = 40;
    CDefClassBase* pa = &a;
    pa->Copy(&b);
    if (g_calls != 1 || g_lhs != &a || g_rhs != &b || a.Base != 30 || a.Value != 40) { printf("004e0a67_TEST FAIL\n"); return 1; }
    printf("004e0a67_TEST PASS\n");
    return 0;
}
