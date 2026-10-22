#pragma optimize("s", on)
extern void __fastcall CTCPreCalculatedNavigationRoute_Destruct(void* route);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCPreCalculatedNavigationRoute_ScalarDeletingDestructor(void* route, int, unsigned int flags)
{
    CTCPreCalculatedNavigationRoute_Destruct(route);
    if (flags & 1)
        OperatorDelete(route);
    return route;
}