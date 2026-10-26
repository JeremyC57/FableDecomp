class CCombatAction_KickThingOnGround { public:
    void* vftable;
};

void __fastcall CCombatAction_KickThingOnGround_dtor(CCombatAction_KickThingOnGround* action);
void operator delete(void*);

void* __fastcall CCombatAction_KickThingOnGround_scalar_deleting_destructor(CCombatAction_KickThingOnGround* action, int edx_unused, unsigned int flags)
{
    CCombatAction_KickThingOnGround_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}