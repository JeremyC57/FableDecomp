// behaviour test for 0x0046284B CThingGroupDef::Copy (model: base part assigned, then the member)
#include <cstdio>
static int g_base = 0;
class CDefClassBase { public: virtual ~CDefClassBase() { } virtual void Copy(const CDefClassBase* other) = 0; };
class CParentDefClassBase : public CDefClassBase { public: int name; CParentDefClassBase& operator=(const CParentDefClassBase& o) { name = o.name; ++g_base; return *this; } };
struct CDef : CParentDefClassBase { long m; virtual void Copy(const CDefClassBase* other) { *this = *static_cast<const CDef*>(other); } };
int main()
{
    CDef a, b; a.name = 1; a.m = 0; b.name = 7; b.m = 42;
    CDefClassBase* p = &a; p->Copy(&b);
    int ok = a.name == 7 && a.m == 42 && g_base == 1;
    if (!ok) { std::printf("0046284b_TEST FAIL\n"); return 1; }
    std::printf("0046284b_TEST PASS\n");
    return 0;
}