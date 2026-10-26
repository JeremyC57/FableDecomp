#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
class CVillageTaskBase
{ public:
    void* vftable;
};
void* const g_CVillageTaskBase_vftable = (void*)0x12345678;
void __fastcall CVillageTaskBase_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CVillageTaskBase* task, int, unsigned int flags)
{
    task->vftable = (void*)&g_CVillageTaskBase_vftable;
    CVillageTaskBase_Destruct(task);
    if (flags & 1)
        operator delete(task);
    return task;
}
int main()
{
    CVillageTaskBase object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CVillageTaskBase_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_006582df_TEST PASS\n");
    return 0;
}