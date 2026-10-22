struct CCreatureAction_ReleaseWillLightningSpell {
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseWillLightningSpell_dtor(CCreatureAction_ReleaseWillLightningSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseWillLightningSpell_scalar_deleting_destructor(CCreatureAction_ReleaseWillLightningSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseWillLightningSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}