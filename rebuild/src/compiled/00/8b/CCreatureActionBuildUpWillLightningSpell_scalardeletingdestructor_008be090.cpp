class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_BuildUpWillLightningSpell { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpWillLightningSpell_scalar_deleting_destructor(CCreatureAction_BuildUpWillLightningSpell* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}