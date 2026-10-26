class CCreatureAction_SpiritHitResponse { public:
    void* vftable;
};

void __fastcall CCreatureAction_SpiritHitResponse_dtor(CCreatureAction_SpiritHitResponse* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SpiritHitResponse_scalar_deleting_destructor(CCreatureAction_SpiritHitResponse* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SpiritHitResponse_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}