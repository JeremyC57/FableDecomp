#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
struct CNymphDef
{
    void* vftable;
};
void* const g_CNymphDef_vftable = (void*)0x12345678;
void __fastcall CNymphDef_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CNymphDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CNymphDef_vftable;
    CNymphDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}
int main()
{
    CNymphDef object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CNymphDef_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d93c4_TEST PASS\n");
    return 0;
}