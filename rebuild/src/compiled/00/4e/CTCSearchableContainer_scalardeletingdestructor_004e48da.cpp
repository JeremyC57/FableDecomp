#pragma optimize("s", on)
extern void __fastcall CTCSearchableContainer_Destruct(void* container);
void* __fastcall CTCSearchableContainer_ScalarDeletingDestructor(void* container, int, unsigned int flags)
{
    CTCSearchableContainer_Destruct(container);
    if (flags & 1)
        operator delete(container);
    return container;
}