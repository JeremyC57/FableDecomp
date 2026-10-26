#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseFood_Destruct(void* food);
void* __fastcall CTCCarriedActionUseFood_ScalarDeletingDestructor(void* food, int, unsigned int flags)
{
    CTCCarriedActionUseFood_Destruct(food);
    if (flags & 1)
        operator delete(food);
    return food;
}