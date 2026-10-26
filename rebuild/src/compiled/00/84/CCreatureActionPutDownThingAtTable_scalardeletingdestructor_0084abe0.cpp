class CCreatureAction_PutDownThingAtTable { public:
    void* vftable;
};

void __fastcall CCreatureAction_PutDownThingAtTable_dtor(CCreatureAction_PutDownThingAtTable* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PutDownThingAtTable_scalar_deleting_destructor(CCreatureAction_PutDownThingAtTable* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PutDownThingAtTable_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}