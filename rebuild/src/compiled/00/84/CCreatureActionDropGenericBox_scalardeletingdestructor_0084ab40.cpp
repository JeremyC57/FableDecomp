struct CCreatureAction_DropGenericBox {
    void* vftable;
};

void __fastcall CCreatureAction_DropGenericBox_dtor(CCreatureAction_DropGenericBox* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DropGenericBox_scalar_deleting_destructor(CCreatureAction_DropGenericBox* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DropGenericBox_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}