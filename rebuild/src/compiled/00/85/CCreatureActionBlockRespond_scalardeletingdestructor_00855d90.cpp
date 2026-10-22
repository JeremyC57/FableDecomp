struct CCreatureAction_BlockRespond {
    void* vftable;
};

void __fastcall CCreatureAction_BlockRespond_dtor(CCreatureAction_BlockRespond* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BlockRespond_scalar_deleting_destructor(CCreatureAction_BlockRespond* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BlockRespond_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}