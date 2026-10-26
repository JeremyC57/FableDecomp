#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
class CTCCameraPointFixedPoint
{ public:
    void* vftable;
};
void* const g_CTCCameraPointFixedPoint_vftable = (void*)0x12345678;
void __fastcall CTCCameraPointFixedPoint_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CTCCameraPointFixedPoint* point, int, unsigned int flags)
{
    point->vftable = (void*)&g_CTCCameraPointFixedPoint_vftable;
    CTCCameraPointFixedPoint_Destruct(point);
    if (flags & 1)
        operator delete(point);
    return point;
}
int main()
{
    CTCCameraPointFixedPoint object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CTCCameraPointFixedPoint_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8203_TEST PASS\n");
    return 0;
}