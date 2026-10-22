struct CCreatureAction_DragonRetreat {
    void* vftable;
};

void __fastcall CCreatureAction_DragonRetreat_dtor(CCreatureAction_DragonRetreat* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonRetreat_scalar_deleting_destructor(CCreatureAction_DragonRetreat* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonRetreat_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}