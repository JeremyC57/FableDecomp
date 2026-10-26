// Test for 0x004E4857 CBalverineBattleDef::Copy(const CDefClassBase*): through the base pointer, assigns the other def to this one
// (exactly one operator= call, with this and the other object).
#include <stdio.h>
static const void* g_lhs; static const void* g_rhs; static int g_calls;
class CDefClassBase {
public:
    int Base;
    virtual ~CDefClassBase() { }
    virtual void Copy(const CDefClassBase* other) { Base = other->Base; }
};
class CBalverineBattleDef : public CDefClassBase {
public:
    int Value;
    CBalverineBattleDef& operator=(const CBalverineBattleDef& other) { ++g_calls; g_lhs = this; g_rhs = &other; Base = other.Base; Value = other.Value; return *this; }
    virtual void Copy(const CDefClassBase* other);
};
void CBalverineBattleDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CBalverineBattleDef*>(other);
}
int main()
{
    CBalverineBattleDef a, b; a.Base = 1; a.Value = 2; b.Base = 30; b.Value = 40;
    CDefClassBase* pa = &a;
    pa->Copy(&b);
    if (g_calls != 1 || g_lhs != &a || g_rhs != &b || a.Base != 30 || a.Value != 40) { printf("004e4857_TEST FAIL\n"); return 1; }
    printf("004e4857_TEST PASS\n");
    return 0;
}
