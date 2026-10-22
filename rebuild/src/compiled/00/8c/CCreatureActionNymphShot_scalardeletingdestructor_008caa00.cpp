struct CCreatureAction_NymphShot {
    void* vftable;
};

void __fastcall CCreatureAction_NymphShot_dtor(CCreatureAction_NymphShot* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphShot_scalar_deleting_destructor(CCreatureAction_NymphShot* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphShot_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}