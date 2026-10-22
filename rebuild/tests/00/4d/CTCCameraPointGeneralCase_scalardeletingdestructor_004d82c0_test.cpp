#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
struct CTCCameraPointGeneralCase
{
    void* vftable;
};
void* const g_CTCCameraPointGeneralCase_vftable = (void*)0x12345678;
void __fastcall CTCCameraPointGeneralCase_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CTCCameraPointGeneralCase* generalCase, int, unsigned int flags)
{
    generalCase->vftable = (void*)&g_CTCCameraPointGeneralCase_vftable;
    CTCCameraPointGeneralCase_Destruct(generalCase);
    if (flags & 1)
        operator delete(generalCase);
    return generalCase;
}
int main()
{
    CTCCameraPointGeneralCase object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CTCCameraPointGeneralCase_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d82c0_TEST PASS\n");
    return 0;
}