#pragma optimize("s", on)
extern void __fastcall CTCSearchableContainer_Destruct(void* container);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSearchableContainer_ScalarDeletingDestructor(void* container, int, unsigned int flags)
{
    CTCSearchableContainer_Destruct(container);
    if (flags & 1)
        OperatorDelete(container);
    return container;
}