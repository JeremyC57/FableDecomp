class CCreatureAction_DragonTakeOffIntoFlying { public:
    void* vftable;
};

void __fastcall CCreatureAction_DragonTakeOffIntoFlying_dtor(CCreatureAction_DragonTakeOffIntoFlying* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonTakeOffIntoFlying_scalar_deleting_destructor(CCreatureAction_DragonTakeOffIntoFlying* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonTakeOffIntoFlying_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}