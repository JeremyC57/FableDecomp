class CCreatureAction_DragonFireBeamOutOf { public:
    void* vftable;
};

void __fastcall CCreatureAction_DragonFireBeamOutOf_dtor(CCreatureAction_DragonFireBeamOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DragonFireBeamOutOf_scalar_deleting_destructor(CCreatureAction_DragonFireBeamOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DragonFireBeamOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}