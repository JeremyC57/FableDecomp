class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_DiagonalStrafeBL { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_DiagonalStrafeBL_scalar_deleting_destructor(CCreatureAction_DiagonalStrafeBL* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}