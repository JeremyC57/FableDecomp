struct CCreatureAction_IceTrollIcicleAttack {
    void* vftable;
};

void __fastcall CCreatureAction_IceTrollIcicleAttack_dtor(CCreatureAction_IceTrollIcicleAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_IceTrollIcicleAttack_scalar_deleting_destructor(CCreatureAction_IceTrollIcicleAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_IceTrollIcicleAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}