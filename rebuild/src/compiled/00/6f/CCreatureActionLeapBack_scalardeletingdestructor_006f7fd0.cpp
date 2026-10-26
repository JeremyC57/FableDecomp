class CCreatureAction_LeapBack { public:
    void* vftable;
};

void __fastcall CCreatureAction_LeapBack_dtor(CCreatureAction_LeapBack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_LeapBack_scalar_deleting_destructor(CCreatureAction_LeapBack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_LeapBack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}