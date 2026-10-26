class CCreatureAction_DragonRearUpOutOf { public:
    void* vftable;
};

void __fastcall CCreatureAction_DragonRearUpOutOf_dtor(CCreatureAction_DragonRearUpOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonRearUpOutOf_scalar_deleting_destructor(CCreatureAction_DragonRearUpOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonRearUpOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}