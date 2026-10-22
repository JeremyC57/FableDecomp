struct CCreatureAction_KrakenLowPoseInto {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenLowPoseInto_dtor(CCreatureAction_KrakenLowPoseInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenLowPoseInto_scalar_deleting_destructor(CCreatureAction_KrakenLowPoseInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenLowPoseInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}