class CCreatureAction_BeetleLand { public:
    void* vftable;
};

void __fastcall CCreatureAction_BeetleLand_dtor(CCreatureAction_BeetleLand* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BeetleLand_scalar_deleting_destructor(CCreatureAction_BeetleLand* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BeetleLand_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}