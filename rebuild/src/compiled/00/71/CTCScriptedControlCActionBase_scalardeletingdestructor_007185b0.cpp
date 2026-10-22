struct CActionBase {
    void* vftable;
};

void __fastcall CActionBase_dtor(CActionBase* action);
void operator delete(void*);

void* __fastcall CActionBase_scalar_deleting_destructor(CActionBase* action, int edx_unused, unsigned int flags)
{
    CActionBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}