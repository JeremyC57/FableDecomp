#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCFlashOnHit_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCFlashOnHit_ScalarDeletingDestructor(void* hit, int, unsigned int flags)
{
    CTCFlashOnHit_Destruct(hit);
    if (flags & 1)
        OperatorDelete(hit);
    return hit;
}
int main()
{
    int object = 0;
    if (CTCFlashOnHit_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCFlashOnHit_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d62d1_TEST PASS\n");
    return 0;
}