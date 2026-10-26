class CCreatureAction_BodgeSideStrike { public:
    void* vftable;
};

void __fastcall CCreatureAction_BodgeSideStrike_dtor(CCreatureAction_BodgeSideStrike* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BodgeSideStrike_scalar_deleting_destructor(CCreatureAction_BodgeSideStrike* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BodgeSideStrike_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}