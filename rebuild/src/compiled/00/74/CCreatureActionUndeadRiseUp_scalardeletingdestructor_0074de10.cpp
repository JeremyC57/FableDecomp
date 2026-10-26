class CCreatureAction_UndeadRiseUp { public:
    void* vftable;
};

void __fastcall CCreatureAction_UndeadRiseUp_dtor(CCreatureAction_UndeadRiseUp* action);
void operator delete(void*);

void* __fastcall CCreatureAction_UndeadRiseUp_scalar_deleting_destructor(CCreatureAction_UndeadRiseUp* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_UndeadRiseUp_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}