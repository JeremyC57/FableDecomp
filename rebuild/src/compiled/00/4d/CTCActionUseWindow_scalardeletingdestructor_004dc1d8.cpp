#pragma optimize("s", on)
extern void __fastcall CTCActionUseWindow_Destruct(void* window);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseWindow_ScalarDeletingDestructor(void* window, int, unsigned int flags)
{
    CTCActionUseWindow_Destruct(window);
    if (flags & 1)
        OperatorDelete(window);
    return window;
}