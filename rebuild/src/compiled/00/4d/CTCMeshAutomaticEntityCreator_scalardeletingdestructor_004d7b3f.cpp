#pragma optimize("s", on)
extern void __fastcall CTCMeshAutomaticEntityCreator_Destruct(void* creator);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCMeshAutomaticEntityCreator_ScalarDeletingDestructor(void* creator, int, unsigned int flags)
{
    CTCMeshAutomaticEntityCreator_Destruct(creator);
    if (flags & 1)
        OperatorDelete(creator);
    return creator;
}