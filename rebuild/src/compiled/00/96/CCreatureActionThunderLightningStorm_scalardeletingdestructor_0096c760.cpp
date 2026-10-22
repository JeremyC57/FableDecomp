struct CCreatureAction_ThunderLightningStorm {
    void* vftable;
};

void __fastcall CCreatureAction_ThunderLightningStorm_dtor(CCreatureAction_ThunderLightningStorm* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ThunderLightningStorm_scalar_deleting_destructor(CCreatureAction_ThunderLightningStorm* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ThunderLightningStorm_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}