class CCreatureAction_PerformExpression { public:
    void* vftable;
};

void __fastcall CCreatureAction_PerformExpression_dtor(CCreatureAction_PerformExpression* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PerformExpression_scalar_deleting_destructor(CCreatureAction_PerformExpression* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PerformExpression_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}