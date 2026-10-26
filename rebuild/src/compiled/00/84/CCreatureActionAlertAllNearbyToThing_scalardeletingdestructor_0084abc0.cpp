class CCreatureAction_AlertAllNearbyToThing { public:
    void* vftable;
};

void __fastcall CCreatureAction_AlertAllNearbyToThing_dtor(CCreatureAction_AlertAllNearbyToThing* action);
void operator delete(void*);

void* __fastcall CCreatureAction_AlertAllNearbyToThing_scalar_deleting_destructor(CCreatureAction_AlertAllNearbyToThing* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_AlertAllNearbyToThing_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}