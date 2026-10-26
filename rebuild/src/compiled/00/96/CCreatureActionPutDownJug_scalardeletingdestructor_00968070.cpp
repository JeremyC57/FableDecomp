class CCreatureAction_PutDownJug { public:
    void* vftable;
};

void __fastcall CCreatureAction_PutDownJug_dtor(CCreatureAction_PutDownJug* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PutDownJug_scalar_deleting_destructor(CCreatureAction_PutDownJug* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PutDownJug_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}