struct CCreatureAction_FodderIdle {
    void* vftable;
};

void __fastcall CCreatureAction_FodderIdle_dtor(CCreatureAction_FodderIdle* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FodderIdle_scalar_deleting_destructor(CCreatureAction_FodderIdle* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FodderIdle_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}