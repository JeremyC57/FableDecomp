struct CCreatureAction_TrollSwipe {
    void* vftable;
};

void __fastcall CCreatureAction_TrollSwipe_dtor(CCreatureAction_TrollSwipe* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollSwipe_scalar_deleting_destructor(CCreatureAction_TrollSwipe* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollSwipe_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}