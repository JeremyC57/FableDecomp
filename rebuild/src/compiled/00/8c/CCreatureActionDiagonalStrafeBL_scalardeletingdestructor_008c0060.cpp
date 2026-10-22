struct CCreatureAction_DiagonalStrafeBL {
    void* vftable;
};

void __fastcall CCreatureAction_DiagonalStrafeBL_dtor(CCreatureAction_DiagonalStrafeBL* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DiagonalStrafeBL_scalar_deleting_destructor(CCreatureAction_DiagonalStrafeBL* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DiagonalStrafeBL_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}