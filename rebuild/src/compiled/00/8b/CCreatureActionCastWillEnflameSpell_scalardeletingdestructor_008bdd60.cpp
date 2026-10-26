class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_CastWillEnflameSpell { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_CastWillEnflameSpell_scalar_deleting_destructor(CCreatureAction_CastWillEnflameSpell* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}