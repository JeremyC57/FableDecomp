class CCreatureActionBase { public:
    void* vftable;
};

void __fastcall CCreatureActionBase_dtor(CCreatureActionBase* action);
void operator delete(void*);

void* __fastcall CCreatureActionBase_scalar_deleting_destructor(CCreatureActionBase* action, int edx_unused, unsigned int flags)
{
    CCreatureActionBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}