class CCreatureAction_ProjectileCrouch { public:
    void* vftable;
};

void __fastcall CCreatureAction_ProjectileCrouch_dtor(CCreatureAction_ProjectileCrouch* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ProjectileCrouch_scalar_deleting_destructor(CCreatureAction_ProjectileCrouch* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ProjectileCrouch_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}