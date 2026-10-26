class CCreatureAction_DragonFlight { public:
    void* vftable;
};

void __fastcall CCreatureAction_DragonFlight_dtor(CCreatureAction_DragonFlight* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonFlight_scalar_deleting_destructor(CCreatureAction_DragonFlight* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonFlight_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}