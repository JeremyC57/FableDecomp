struct CCreatureAction_UnsheatheItemFromInventoryWithHasteAction {
    void* vftable;
};

void __fastcall CCreatureAction_UnsheatheItemFromInventoryWithHasteAction_dtor(CCreatureAction_UnsheatheItemFromInventoryWithHasteAction* action);
void operator delete(void*);

void* __fastcall CCreatureAction_UnsheatheItemFromInventoryWithHasteAction_scalar_deleting_destructor(CCreatureAction_UnsheatheItemFromInventoryWithHasteAction* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_UnsheatheItemFromInventoryWithHasteAction_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}