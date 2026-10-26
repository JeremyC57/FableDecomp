class CCreatureAction_YellOverHere { public:
    void* vftable;
};

void __fastcall CCreatureAction_YellOverHere_dtor(CCreatureAction_YellOverHere* action);
void operator delete(void*);

void* __fastcall CCreatureAction_YellOverHere_scalar_deleting_destructor(CCreatureAction_YellOverHere* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_YellOverHere_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}