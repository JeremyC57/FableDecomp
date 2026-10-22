struct CCreatureAction_TrollIdle {
    void* vftable;
};

void __fastcall CCreatureAction_TrollIdle_dtor(CCreatureAction_TrollIdle* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollIdle_scalar_deleting_destructor(CCreatureAction_TrollIdle* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollIdle_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}