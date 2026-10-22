struct CActionWait {
    void* vftable;
};

void __fastcall CActionWait_dtor(CActionWait* action);
void operator delete(void*);

void* __fastcall CActionWait_scalar_deleting_destructor(CActionWait* action, int edx_unused, unsigned int flags)
{
    CActionWait_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}