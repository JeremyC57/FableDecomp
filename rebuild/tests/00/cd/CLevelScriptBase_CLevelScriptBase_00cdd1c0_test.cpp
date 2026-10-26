// behaviour test for 0x00CDD1C0 CLevelScriptBase<NScript::CEmptyData>::CLevelScriptBase(CScriptDataBase*, CGameScriptInterfaceBase&): the base part is constructed first, then the two arguments land in their own members (Game from the second argument, MasterScriptData from the first argument)
#include <cstdio>
static int gOrder = 0, gBaseOrder = 0;
struct Base { int state; Base() : state(7) { gBaseOrder = ++gOrder; } virtual ~Base() { } };
struct Object : Base {
    void* Game; void* MasterScriptData;
    Object(void* a1, void* a2) : Game(a2), MasterScriptData(a1) { }      // the function under test
};
int main()
{
    int fail = 0;
    int x = 1, y = 2;
    gOrder = 0;
    Object o(&x, &y);
    if (gBaseOrder != 1 || o.state != 7) fail |= 1;
    if (o.Game != (void*)&y || o.MasterScriptData != (void*)&x) fail |= 2;
    if (fail) { std::printf("00cdd1c0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00cdd1c0_TEST PASS\n");
    return 0;
}