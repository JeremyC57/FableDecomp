struct CCreatureAction_CastSkillMultiArrow {
    void* vftable;
};

void __fastcall CCreatureAction_CastSkillMultiArrow_dtor(CCreatureAction_CastSkillMultiArrow* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastSkillMultiArrow_scalar_deleting_destructor(CCreatureAction_CastSkillMultiArrow* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastSkillMultiArrow_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}