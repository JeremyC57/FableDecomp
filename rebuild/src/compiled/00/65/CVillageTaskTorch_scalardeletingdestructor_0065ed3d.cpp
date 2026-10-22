#pragma optimize("s", on)
extern void __fastcall CVillageTaskTorch_Destruct(void* task);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CVillageTaskTorch_ScalarDeletingDestructor(void* task, int, unsigned int flags)
{
    CVillageTaskTorch_Destruct(task);
    if (flags & 1)
        OperatorDelete(task);
    return task;
}