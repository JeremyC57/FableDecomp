#pragma optimize("s", on)
extern void __fastcall CTCPreCalculatedNavigationRoute_Destruct(void* route);
void* __fastcall CTCPreCalculatedNavigationRoute_ScalarDeletingDestructor(void* route, int, unsigned int flags)
{
    CTCPreCalculatedNavigationRoute_Destruct(route);
    if (flags & 1)
        operator delete(route);
    return route;
}