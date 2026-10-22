struct CCreatureAction_DragonFireBlastBase {
    void* vftable;
};

void __fastcall CCreatureAction_DragonFireBlastBase_dtor(CCreatureAction_DragonFireBlastBase* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonFireBlastBase_scalar_deleting_destructor(CCreatureAction_DragonFireBlastBase* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonFireBlastBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}