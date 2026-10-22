#pragma optimize("s", on)
extern void __fastcall CTCSwitchableNavigation_Destruct(void* navigation);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSwitchableNavigation_ScalarDeletingDestructor(void* navigation, int, unsigned int flags)
{
    CTCSwitchableNavigation_Destruct(navigation);
    if (flags & 1)
        OperatorDelete(navigation);
    return navigation;
}