class CCreatureAction_BriarRoseShotFarInto { public:
    void* vftable;
};

void __fastcall CCreatureAction_BriarRoseShotFarInto_dtor(CCreatureAction_BriarRoseShotFarInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BriarRoseShotFarInto_scalar_deleting_destructor(CCreatureAction_BriarRoseShotFarInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BriarRoseShotFarInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}