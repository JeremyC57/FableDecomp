struct CCreatureAction_NymphGetHitDie {
    void* vftable;
};

void __fastcall CCreatureAction_NymphGetHitDie_dtor(CCreatureAction_NymphGetHitDie* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphGetHitDie_scalar_deleting_destructor(CCreatureAction_NymphGetHitDie* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphGetHitDie_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}