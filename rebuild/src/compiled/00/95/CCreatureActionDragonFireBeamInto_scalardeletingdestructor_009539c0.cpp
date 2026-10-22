struct CCreatureAction_DragonFireBeamInto {
    void* vftable;
};

void __fastcall CCreatureAction_DragonFireBeamInto_dtor(CCreatureAction_DragonFireBeamInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonFireBeamInto_scalar_deleting_destructor(CCreatureAction_DragonFireBeamInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonFireBeamInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}