struct CCreatureAction_AddFoodToTable {
    void* vftable;
};

void __fastcall CCreatureAction_AddFoodToTable_dtor(CCreatureAction_AddFoodToTable* action);
void operator delete(void*);

void* __fastcall CCreatureAction_AddFoodToTable_scalar_deleting_destructor(CCreatureAction_AddFoodToTable* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_AddFoodToTable_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}