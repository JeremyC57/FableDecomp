class CCreatureAction_TrollGetProjectile { public:
    void* vftable;
};

void __fastcall CCreatureAction_TrollGetProjectile_dtor(CCreatureAction_TrollGetProjectile* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollGetProjectile_scalar_deleting_destructor(CCreatureAction_TrollGetProjectile* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollGetProjectile_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}