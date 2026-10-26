class CCreatureAction_GenericLoadProjectileWeapon { public:
    void* vftable;
};

void __fastcall CCreatureAction_GenericLoadProjectileWeapon_dtor(CCreatureAction_GenericLoadProjectileWeapon* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GenericLoadProjectileWeapon_scalar_deleting_destructor(CCreatureAction_GenericLoadProjectileWeapon* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GenericLoadProjectileWeapon_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}