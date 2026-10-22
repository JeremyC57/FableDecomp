struct CCreatureAction_KrakenRiseUp {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenRiseUp_dtor(CCreatureAction_KrakenRiseUp* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenRiseUp_scalar_deleting_destructor(CCreatureAction_KrakenRiseUp* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenRiseUp_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}