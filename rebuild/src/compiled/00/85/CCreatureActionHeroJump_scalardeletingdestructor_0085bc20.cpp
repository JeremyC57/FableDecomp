class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureActionHeroJump { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureActionHeroJump_scalar_deleting_destructor(CCreatureActionHeroJump* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}