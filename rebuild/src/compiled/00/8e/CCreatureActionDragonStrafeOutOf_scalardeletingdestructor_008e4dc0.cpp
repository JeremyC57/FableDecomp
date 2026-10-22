struct CCreatureAction_DragonStrafeOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_DragonStrafeOutOf_dtor(CCreatureAction_DragonStrafeOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonStrafeOutOf_scalar_deleting_destructor(CCreatureAction_DragonStrafeOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonStrafeOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}