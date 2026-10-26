#pragma optimize("s", on)
extern void __fastcall CTCSmashableWindow_Destruct(void* window);
void* __fastcall CTCSmashableWindow_ScalarDeletingDestructor(void* window, int, unsigned int flags)
{
    CTCSmashableWindow_Destruct(window);
    if (flags & 1)
        operator delete(window);
    return window;
}