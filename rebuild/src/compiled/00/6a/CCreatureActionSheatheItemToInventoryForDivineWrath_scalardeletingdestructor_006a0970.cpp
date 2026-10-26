class CCreatureAction_SheatheItemToInventory_ForDivineWrath { public:
    void* vftable;
};

void __fastcall CCreatureAction_SheatheItemToInventory_ForDivineWrath_dtor(CCreatureAction_SheatheItemToInventory_ForDivineWrath* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SheatheItemToInventory_ForDivineWrath_scalar_deleting_destructor(CCreatureAction_SheatheItemToInventory_ForDivineWrath* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SheatheItemToInventory_ForDivineWrath_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}