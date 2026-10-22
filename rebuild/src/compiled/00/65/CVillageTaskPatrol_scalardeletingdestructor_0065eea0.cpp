#pragma optimize("s", on)
extern void __fastcall CVillageTaskPatrol_Destruct(void* task);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CVillageTaskPatrol_ScalarDeletingDestructor(void* task, int, unsigned int flags)
{
    CVillageTaskPatrol_Destruct(task);
    if (flags & 1)
        OperatorDelete(task);
    return task;
}