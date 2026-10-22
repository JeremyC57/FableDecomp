struct CCreatureAction_ShowFear {
    void* vftable;
};

void __fastcall CCreatureAction_ShowFear_dtor(CCreatureAction_ShowFear* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ShowFear_scalar_deleting_destructor(CCreatureAction_ShowFear* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ShowFear_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}