#pragma optimize("s", on)
extern void __fastcall CTCSmashableWindow_Destruct(void* window);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSmashableWindow_ScalarDeletingDestructor(void* window, int, unsigned int flags)
{
    CTCSmashableWindow_Destruct(window);
    if (flags & 1)
        OperatorDelete(window);
    return window;
}