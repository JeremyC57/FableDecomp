class CCreatureActionHeroJump { public:
    void* vftable;
};

void __fastcall CCreatureActionHeroJump_dtor(CCreatureActionHeroJump* action);
void operator delete(void*);

void* __fastcall CCreatureActionHeroJump_scalar_deleting_destructor(CCreatureActionHeroJump* action, int edx_unused, unsigned int flags)
{
    CCreatureActionHeroJump_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}