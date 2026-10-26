class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_CastWillHarvestSoulSpell { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_CastWillHarvestSoulSpell_scalar_deleting_destructor(CCreatureAction_CastWillHarvestSoulSpell* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}