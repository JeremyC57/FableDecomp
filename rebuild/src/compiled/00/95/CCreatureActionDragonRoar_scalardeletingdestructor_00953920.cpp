struct CCreatureAction_DragonRoar {
    void* vftable;
};

void __fastcall CCreatureAction_DragonRoar_dtor(CCreatureAction_DragonRoar* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonRoar_scalar_deleting_destructor(CCreatureAction_DragonRoar* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonRoar_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}