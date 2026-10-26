class CCreatureAction_ScreamerBackOff { public:
    void* vftable;
};

void __fastcall CCreatureAction_ScreamerBackOff_dtor(CCreatureAction_ScreamerBackOff* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScreamerBackOff_scalar_deleting_destructor(CCreatureAction_ScreamerBackOff* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScreamerBackOff_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}