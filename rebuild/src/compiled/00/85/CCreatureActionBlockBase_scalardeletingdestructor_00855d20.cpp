struct CCreatureAction_BlockBase {
    void* vftable;
};

void __fastcall CCreatureAction_BlockBase_dtor(CCreatureAction_BlockBase* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BlockBase_scalar_deleting_destructor(CCreatureAction_BlockBase* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BlockBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}