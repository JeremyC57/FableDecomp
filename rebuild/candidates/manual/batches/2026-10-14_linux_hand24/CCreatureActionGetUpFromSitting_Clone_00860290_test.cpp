#include <cstdio>
#include <cstring>
struct CCreatureActionBase;
static int g_baseCopies;
struct CCombatActionBase { virtual void v00() { } unsigned char _body[0x114 - 4];
    CCombatActionBase() { std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(const CCombatActionBase& o) { ++g_baseCopies; std::memcpy(_body, o._body, sizeof(_body)); } };
struct CCreatureAction_GetUpFromSitting : CCombatActionBase {
    long SittingMode; unsigned char RemoveOnPerformAction;
    virtual void v00() { }
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_GetUpFromSitting::Clone() const { return (CCreatureActionBase*)new CCreatureAction_GetUpFromSitting(*this); }
int main()
{
    CCreatureAction_GetUpFromSitting a; a._body[3] = 42; a.SittingMode = 7; a.RemoveOnPerformAction = 1;
    CCreatureAction_GetUpFromSitting* b = (CCreatureAction_GetUpFromSitting*)a.Clone();
    if (b == 0 || b == &a || g_baseCopies != 1 || b->_body[3] != 42 || sizeof(a) != 0x11c || !(b->SittingMode == 7 && b->RemoveOnPerformAction == 1)) { std::printf("00860290_TEST FAIL\n"); return 1; }
    delete b;
    std::printf("00860290_TEST PASS\n");
    return 0;
}
