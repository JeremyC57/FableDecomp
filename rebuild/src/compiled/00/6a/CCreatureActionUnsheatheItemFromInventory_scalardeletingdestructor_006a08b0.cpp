struct CCreatureAction_UnsheatheItemFromInventory {
    void* vftable;
};

void __fastcall CCreatureAction_UnsheatheItemFromInventory_dtor(CCreatureAction_UnsheatheItemFromInventory* action);
void operator delete(void*);

void* __fastcall CCreatureAction_UnsheatheItemFromInventory_scalar_deleting_destructor(CCreatureAction_UnsheatheItemFromInventory* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_UnsheatheItemFromInventory_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}