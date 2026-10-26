class CCreatureAction_DragonFireBlastLeft { public:
    void* vftable;
};

void __fastcall CCreatureAction_DragonFireBlastLeft_dtor(CCreatureAction_DragonFireBlastLeft* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonFireBlastLeft_scalar_deleting_destructor(CCreatureAction_DragonFireBlastLeft* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonFireBlastLeft_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}