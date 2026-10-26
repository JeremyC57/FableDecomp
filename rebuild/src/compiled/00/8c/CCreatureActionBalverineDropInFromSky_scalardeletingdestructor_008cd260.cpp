class CCreatureAction_BalverineDropInFromSky { public:
    void* vftable;
};

void __fastcall CCreatureAction_BalverineDropInFromSky_dtor(CCreatureAction_BalverineDropInFromSky* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BalverineDropInFromSky_scalar_deleting_destructor(CCreatureAction_BalverineDropInFromSky* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BalverineDropInFromSky_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}