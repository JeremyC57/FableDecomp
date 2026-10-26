class CCreatureAction_NymphTeleportLimboBase { public:
    void* vftable;
};

void __fastcall CCreatureAction_NymphTeleportLimboBase_dtor(CCreatureAction_NymphTeleportLimboBase* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphTeleportLimboBase_scalar_deleting_destructor(CCreatureAction_NymphTeleportLimboBase* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphTeleportLimboBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}