#pragma optimize("s", on)
extern void __fastcall CVillageTaskCarryCrate_Destruct(void* task);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CVillageTaskCarryCrate_ScalarDeletingDestructor(void* task, int, unsigned int flags)
{
    CVillageTaskCarryCrate_Destruct(task);
    if (flags & 1)
        OperatorDelete(task);
    return task;
}