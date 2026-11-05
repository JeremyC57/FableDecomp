#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
class CBettingDef
{ public:
    void* vftable;
};
void* const g_CBettingDef_vftable = (void*)0x12345678;
void __fastcall CBettingDef_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall CBettingDef_scalar_deleting_destructor(CBettingDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CBettingDef_vftable;
    CBettingDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}
int main()
{
    CBettingDef object = {0};
    if (CBettingDef_scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CBettingDef_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    CBettingDef_scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d9701_TEST PASS\n");
    return 0;
}