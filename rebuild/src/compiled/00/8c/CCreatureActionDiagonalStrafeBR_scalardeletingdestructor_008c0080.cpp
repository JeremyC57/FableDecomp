class CCreatureAction_DiagonalStrafeBR { public:
    void* vftable;
};

void __fastcall CCreatureAction_DiagonalStrafeBR_dtor(CCreatureAction_DiagonalStrafeBR* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DiagonalStrafeBR_scalar_deleting_destructor(CCreatureAction_DiagonalStrafeBR* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DiagonalStrafeBR_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}