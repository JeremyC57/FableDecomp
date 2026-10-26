#pragma optimize("s", on)
extern void __fastcall CTCActionUseWindow_Destruct(void* window);
void* __fastcall CTCActionUseWindow_ScalarDeletingDestructor(void* window, int, unsigned int flags)
{
    CTCActionUseWindow_Destruct(window);
    if (flags & 1)
        operator delete(window);
    return window;
}