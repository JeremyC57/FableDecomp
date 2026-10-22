struct CCombatActionBase {
    void* vftable;
};

void __fastcall CCombatActionBase_dtor(CCombatActionBase* action);
void operator delete(void*);

void* __fastcall CCombatActionBase_scalar_deleting_destructor(CCombatActionBase* action, int edx_unused, unsigned int flags)
{
    CCombatActionBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}