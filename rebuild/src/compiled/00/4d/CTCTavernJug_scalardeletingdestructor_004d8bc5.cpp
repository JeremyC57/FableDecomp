#pragma optimize("s", on)
extern void __fastcall CTCTavernJug_Destruct(void* jug);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTavernJug_ScalarDeletingDestructor(void* jug, int, unsigned int flags)
{
    CTCTavernJug_Destruct(jug);
    if (flags & 1)
        OperatorDelete(jug);
    return jug;
}