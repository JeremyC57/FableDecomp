#pragma optimize("s", on)
class CVillageTaskBase
{ public:
    void* vftable;
};
extern void* const g_CVillageTaskBase_vftable;
extern void __fastcall CVillageTaskBase_Destruct(void* task);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CVillageTaskBase* task, int, unsigned int flags)
{
    task->vftable = (void*)&g_CVillageTaskBase_vftable;
    CVillageTaskBase_Destruct(task);
    if (flags & 1)
        operator delete(task);
    return task;
}