struct CCreatureAction_DragonRearUpInto {
    void* vftable;
};

void __fastcall CCreatureAction_DragonRearUpInto_dtor(CCreatureAction_DragonRearUpInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonRearUpInto_scalar_deleting_destructor(CCreatureAction_DragonRearUpInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonRearUpInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}