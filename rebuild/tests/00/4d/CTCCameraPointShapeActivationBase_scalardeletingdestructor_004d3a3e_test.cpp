#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
struct CTCCameraPointShapeActivationBase
{
    void* vftable;
};
void* const g_CTCCameraPointShapeActivationBase_vftable = (void*)0x12345678;
void __fastcall CTCCameraPointShapeActivationBase_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CTCCameraPointShapeActivationBase* activation, int, unsigned int flags)
{
    activation->vftable = (void*)&g_CTCCameraPointShapeActivationBase_vftable;
    CTCCameraPointShapeActivationBase_Destruct(activation);
    if (flags & 1)
        operator delete(activation);
    return activation;
}
int main()
{
    CTCCameraPointShapeActivationBase object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CTCCameraPointShapeActivationBase_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d3a3e_TEST PASS\n");
    return 0;
}