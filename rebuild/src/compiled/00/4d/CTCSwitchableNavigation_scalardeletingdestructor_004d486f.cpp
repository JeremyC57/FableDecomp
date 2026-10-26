#pragma optimize("s", on)
extern void __fastcall CTCSwitchableNavigation_Destruct(void* navigation);
void* __fastcall CTCSwitchableNavigation_ScalarDeletingDestructor(void* navigation, int, unsigned int flags)
{
    CTCSwitchableNavigation_Destruct(navigation);
    if (flags & 1)
        operator delete(navigation);
    return navigation;
}