struct CCreatureAction_LeapLong {
    void* vftable;
};

void __fastcall CCreatureAction_LeapLong_dtor(CCreatureAction_LeapLong* action);
void operator delete(void*);

void* __fastcall CCreatureAction_LeapLong_scalar_deleting_destructor(CCreatureAction_LeapLong* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_LeapLong_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}