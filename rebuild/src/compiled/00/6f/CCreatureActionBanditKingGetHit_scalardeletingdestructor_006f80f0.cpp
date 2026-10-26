class CCreatureAction_BanditKingGetHit { public:
    void* vftable;
};

void __fastcall CCreatureAction_BanditKingGetHit_dtor(CCreatureAction_BanditKingGetHit* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BanditKingGetHit_scalar_deleting_destructor(CCreatureAction_BanditKingGetHit* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BanditKingGetHit_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}