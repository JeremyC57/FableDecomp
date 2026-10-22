struct CCreatureAction_BanditKingBackOff {
    void* vftable;
};

void __fastcall CCreatureAction_BanditKingBackOff_dtor(CCreatureAction_BanditKingBackOff* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BanditKingBackOff_scalar_deleting_destructor(CCreatureAction_BanditKingBackOff* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BanditKingBackOff_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}