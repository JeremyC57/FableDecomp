struct CCreatureAction_DragonHoverOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_DragonHoverOutOf_dtor(CCreatureAction_DragonHoverOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonHoverOutOf_scalar_deleting_destructor(CCreatureAction_DragonHoverOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonHoverOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}