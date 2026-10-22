#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
struct CShoveHaPennyRegionDef
{
    void* vftable;
};
void* const g_CShoveHaPennyRegionDef_vftable = (void*)0x12345678;
void __fastcall CShoveHaPennyRegionDef_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CShoveHaPennyRegionDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CShoveHaPennyRegionDef_vftable;
    CShoveHaPennyRegionDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}
int main()
{
    CShoveHaPennyRegionDef object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CShoveHaPennyRegionDef_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004eafa7_TEST PASS\n");
    return 0;
}