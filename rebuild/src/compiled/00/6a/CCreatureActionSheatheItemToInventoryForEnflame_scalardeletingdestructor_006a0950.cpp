struct CCreatureAction_SheatheItemToInventory_ForEnflame {
    void* vftable;
};

void __fastcall CCreatureAction_SheatheItemToInventory_ForEnflame_dtor(CCreatureAction_SheatheItemToInventory_ForEnflame* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SheatheItemToInventory_ForEnflame_scalar_deleting_destructor(CCreatureAction_SheatheItemToInventory_ForEnflame* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SheatheItemToInventory_ForEnflame_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}