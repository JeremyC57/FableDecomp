struct CCreatureAction_DragonFireBlastForward {
    void* vftable;
};

void __fastcall CCreatureAction_DragonFireBlastForward_dtor(CCreatureAction_DragonFireBlastForward* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonFireBlastForward_scalar_deleting_destructor(CCreatureAction_DragonFireBlastForward* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonFireBlastForward_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}