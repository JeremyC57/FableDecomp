class CCreatureAction_StopEatingAtTable { public:
    void* vftable;
};

void __fastcall CCreatureAction_StopEatingAtTable_dtor(CCreatureAction_StopEatingAtTable* action);
void operator delete(void*);

void* __fastcall CCreatureAction_StopEatingAtTable_scalar_deleting_destructor(CCreatureAction_StopEatingAtTable* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_StopEatingAtTable_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}