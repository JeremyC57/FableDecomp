class CCreatureAction_GenericFireProjectileWeapon { public:
    void* vftable;
};

void __fastcall CCreatureAction_GenericFireProjectileWeapon_dtor(CCreatureAction_GenericFireProjectileWeapon* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GenericFireProjectileWeapon_scalar_deleting_destructor(CCreatureAction_GenericFireProjectileWeapon* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GenericFireProjectileWeapon_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}