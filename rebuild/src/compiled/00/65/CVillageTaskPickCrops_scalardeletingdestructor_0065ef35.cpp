#pragma optimize("s", on)
extern void __fastcall CVillageTaskPickCrops_Destruct(void* task);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CVillageTaskPickCrops_ScalarDeletingDestructor(void* task, int, unsigned int flags)
{
    CVillageTaskPickCrops_Destruct(task);
    if (flags & 1)
        OperatorDelete(task);
    return task;
}