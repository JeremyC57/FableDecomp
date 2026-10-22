struct CCreatureAction_DragonSwoop {
    void* vftable;
};

void __fastcall CCreatureAction_DragonSwoop_dtor(CCreatureAction_DragonSwoop* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonSwoop_scalar_deleting_destructor(CCreatureAction_DragonSwoop* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonSwoop_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}