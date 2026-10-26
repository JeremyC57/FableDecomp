class CCreatureAction_DragonLand { public:
    void* vftable;
};

void __fastcall CCreatureAction_DragonLand_dtor(CCreatureAction_DragonLand* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonLand_scalar_deleting_destructor(CCreatureAction_DragonLand* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonLand_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}