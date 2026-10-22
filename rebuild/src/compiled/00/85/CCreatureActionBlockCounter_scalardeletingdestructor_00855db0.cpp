struct CCreatureAction_BlockCounter {
    void* vftable;
};

void __fastcall CCreatureAction_BlockCounter_dtor(CCreatureAction_BlockCounter* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BlockCounter_scalar_deleting_destructor(CCreatureAction_BlockCounter* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BlockCounter_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}