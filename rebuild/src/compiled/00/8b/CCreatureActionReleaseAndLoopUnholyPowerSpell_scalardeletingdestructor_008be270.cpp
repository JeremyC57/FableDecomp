struct CCreatureAction_ReleaseAndLoopUnholyPowerSpell {
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseAndLoopUnholyPowerSpell_dtor(CCreatureAction_ReleaseAndLoopUnholyPowerSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseAndLoopUnholyPowerSpell_scalar_deleting_destructor(CCreatureAction_ReleaseAndLoopUnholyPowerSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseAndLoopUnholyPowerSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}