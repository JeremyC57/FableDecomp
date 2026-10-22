#pragma optimize("s", on)
extern void __fastcall CVillageTaskHorn_Destruct(void* task);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CVillageTaskHorn_ScalarDeletingDestructor(void* task, int, unsigned int flags)
{
    CVillageTaskHorn_Destruct(task);
    if (flags & 1)
        OperatorDelete(task);
    return task;
}