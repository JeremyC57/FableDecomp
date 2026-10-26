class CCreatureAction_NymphGetHit { public:
    void* vftable;
};

void __fastcall CCreatureAction_NymphGetHit_dtor(CCreatureAction_NymphGetHit* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphGetHit_scalar_deleting_destructor(CCreatureAction_NymphGetHit* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphGetHit_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}