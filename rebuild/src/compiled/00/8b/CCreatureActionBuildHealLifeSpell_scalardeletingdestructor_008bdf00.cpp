class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_BuildHealLifeSpell { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_BuildHealLifeSpell_scalar_deleting_destructor(CCreatureAction_BuildHealLifeSpell* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}