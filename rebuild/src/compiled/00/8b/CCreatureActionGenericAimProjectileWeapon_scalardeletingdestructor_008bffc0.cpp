class CCreatureAction_GenericAimProjectileWeapon { public:
    void* vftable;
};

void __fastcall CCreatureAction_GenericAimProjectileWeapon_dtor(CCreatureAction_GenericAimProjectileWeapon* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GenericAimProjectileWeapon_scalar_deleting_destructor(CCreatureAction_GenericAimProjectileWeapon* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GenericAimProjectileWeapon_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}