class CCreatureAction_DragonHoverInto { public:
    void* vftable;
};

void __fastcall CCreatureAction_DragonHoverInto_dtor(CCreatureAction_DragonHoverInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonHoverInto_scalar_deleting_destructor(CCreatureAction_DragonHoverInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonHoverInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}