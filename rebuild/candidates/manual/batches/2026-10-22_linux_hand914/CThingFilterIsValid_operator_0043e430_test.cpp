// Behaviour test for 0043e430 CThingFilter_IsValid::operator() (forwards to virtual slot 1)
#include <cstdio>
static long g_value = 42;
struct CThingFilter_IsValid {
    virtual void  Slot00() {}
    virtual long* Slot01() { return &g_value; }
    long* operator()();
    virtual ~CThingFilter_IsValid() {}
};
long* CThingFilter_IsValid::operator()()
{
    return Slot01();
}
static long g_other = 7;
struct CDerived : CThingFilter_IsValid { virtual long* Slot01() { return &g_other; } };
int main()
{
    CThingFilter_IsValid f; CDerived d;
    if (f() != &g_value || d() != &g_other) { std::printf("0043e430_TEST FAIL\n"); return 1; }
    std::printf("0043e430_TEST PASS\n");
    return 0;
}
